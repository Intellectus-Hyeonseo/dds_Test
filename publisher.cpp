#include <int2dds/dds/domain/DomainParticipantFactory.hpp>
#include <int2dds/dds/domain/DomainParticipant.hpp>
#include <int2dds/dds/topic/Topic.hpp>
#include <int2dds/dds/publisher/Publisher.hpp>
#include <int2dds/dds/publisher/DataWriter.hpp>
#include <int2dds/dds/publisher/qos/DataWriterQos.hpp>
#include <int2dds/dds/topic/TypeSupport.hpp>
#include "headers/dds_TestPubSubTypes.hpp"

#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>

using namespace intellectus::int2dds::dds;

int main()
{
    // DomainParticipant 생성
    DomainParticipantQos participant_qos;
    DomainParticipant* participant = DomainParticipantFactory::get_instance()->create_participant(0, participant_qos);

    // Type 등록
    TypeSupport type(new ImagePubSubType());
    type.register_type(participant);

    // Topic 생성
    Topic* topic = participant->create_topic("ImageTopic", type.get_type_name(), TOPIC_QOS_DEFAULT);

    // Publisher 생성
    Publisher* publisher = participant->create_publisher(PUBLISHER_QOS_DEFAULT);

    // DataWriter 생성
    DataWriterQos writer_qos = DATAWRITER_QOS_DEFAULT;
    DataWriter* writer = publisher->create_datawriter(topic, writer_qos);

    // OpenCV 카메라 열기
    cv::VideoCapture cap(1);
    if (!cap.isOpened()) {
        std::cerr << "카메라를 열 수 없습니다." << std::endl;
        return 1;
    }

    uint32_t index = 0;
    cv::Mat frame;
    std::vector<uchar> buffer;

    while (cv::waitKey(1) != 27) // ESC 키를 누르면 종료
    {
        cap >> frame;
        if (frame.empty()) continue;

        // JPEG 압축
        buffer.clear();
        cv::imencode(".jpg", frame, buffer);

        // 메시지 생성 및 발행
        Image image;
        image.frame_index(index++);
        image.format("jpeg");
        image.size(static_cast<uint32_t>(buffer.size()));
        image.frame().assign(buffer.begin(), buffer.end());

        writer->write(&image);

        std::cout << "Published frame index: " << image.frame_index() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // 약 30fps
    }

    // Clean up
    cap.release();
    participant->delete_contained_entities();
    DomainParticipantFactory::get_instance()->delete_participant(participant);

    return 0;
}
