#include <int2dds/dds/domain/DomainParticipantFactory.hpp>
#include <int2dds/dds/domain/DomainParticipant.hpp>
#include <int2dds/dds/topic/Topic.hpp>
#include <int2dds/dds/subscriber/Subscriber.hpp>
#include <int2dds/dds/subscriber/DataReader.hpp>
#include <int2dds/dds/subscriber/qos/DataReaderQos.hpp>
#include <int2dds/dds/topic/TypeSupport.hpp>
#include <int2dds/dds/subscriber/SampleInfo.hpp>
#include <int2dds/dds/core/ReturnCode.hpp>
#include <int2dds/dds/core/detail/DDSReturnCode.hpp>
#include "headers/dds_TestPubSubTypes.hpp"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <atomic>
#include <cmath>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <fstream>

using namespace intellectus::int2dds::dds;

struct FrameInfo {
    std::vector<uchar> data;
    uint32_t frame_index;
    std::chrono::steady_clock::time_point arrival_time;
};

class ImageListener : public DataReaderListener
{
public:
    std::queue<FrameInfo> frame_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;

    void on_data_available(DataReader* reader) override
    {
        Image image;
        SampleInfo info;
        if (reader->take_next_sample(&image, &info) == RETCODE_OK && info.instance_state == ALIVE_INSTANCE_STATE)
        {
            FrameInfo frame_info;
            frame_info.data = image.frame();
            frame_info.frame_index = image.frame_index();
            frame_info.arrival_time = std::chrono::steady_clock::now();

            std::lock_guard<std::mutex> lock(queue_mutex);
            frame_queue.push(std::move(frame_info));
            queue_cv.notify_one();
        }
    }
};

void display_loop(ImageListener& listener)
{
    std::ofstream log_file("frame_stats.txt");  // 로그 파일 생성
    if (!log_file.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
        return;
    }

    std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point last_fps_time = std::chrono::steady_clock::now();
    double last_iat_ms = 0.0;
    int frame_count = 0;
    double fps = 0.0;

    while (true)
    {
        std::unique_lock<std::mutex> lock(listener.queue_mutex);
        listener.queue_cv.wait(lock, [&] { return !listener.frame_queue.empty(); });

        FrameInfo frame = std::move(listener.frame_queue.front());
        listener.frame_queue.pop();
        lock.unlock();

        // 시간 측정
        auto now = frame.arrival_time;
        double iat_ms = std::chrono::duration<double, std::milli>(now - last_time).count();
        double jitter = std::abs(iat_ms - last_iat_ms);
        last_time = now;
        last_iat_ms = iat_ms;

        // FPS 계산
        ++frame_count;
        auto elapsed_sec = std::chrono::duration_cast<std::chrono::seconds>(now - last_fps_time).count();
        if (elapsed_sec >= 1) {
            fps = frame_count / static_cast<double>(elapsed_sec);
            frame_count = 0;
            last_fps_time = now;
        }

        // 로그 저장
        log_file << "FrameIndex: " << frame.frame_index
                 << ", IAT: " << iat_ms << " ms"
                 << ", Jitter: " << jitter << " ms"
                 << ", FPS: " << fps << std::endl;

        // 화면 출력
        cv::Mat decoded(480, 640, CV_8UC3, frame.data.data());
        if (!decoded.empty()) {
            cv::putText(decoded, "FPS: " + std::to_string(static_cast<int>(std::round(fps))), cv::Point(10, 30),
                        cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
            cv::putText(decoded, "Jitter: " + std::to_string(static_cast<int>(jitter)) + " ms", cv::Point(10, 60),
                        cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 2);
            cv::putText(decoded, "IAT: " + std::to_string(static_cast<int>(iat_ms)) + " ms", cv::Point(10, 90),
                        cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 0, 0), 2);

            cv::imshow("Received Frame", decoded);
            if (cv::waitKey(1) == 27) break;  // ESC
        }
    }

    log_file.close();  // 파일 닫기
}

int main()
{
    DomainParticipant* participant = DomainParticipantFactory::get_instance()->create_participant(0, PARTICIPANT_QOS_DEFAULT);
    TypeSupport type(new ImagePubSubType());
    type.register_type(participant);

    Topic* topic = participant->create_topic("ImageTopic", type.get_type_name(), TOPIC_QOS_DEFAULT);
    Subscriber* subscriber = participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT);

    ImageListener listener;
    subscriber->create_datareader(topic, DATAREADER_QOS_DEFAULT, &listener);

    std::thread display_thread(display_loop, std::ref(listener));
    display_thread.join();

    cv::destroyAllWindows();
    participant->delete_contained_entities();
    DomainParticipantFactory::get_instance()->delete_participant(participant);

    return 0;
}
