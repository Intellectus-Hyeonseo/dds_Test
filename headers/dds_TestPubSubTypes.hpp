// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (Intellectus).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file dds_TestPubSubTypes.hpp
 * This header file contains the declaration of the serialization functions.
 *
 * This file was generated by the tool int2ddsgen (version: 4.1.0).
 */


#ifndef INT2_DDS_GENERATED__DDS_TEST_PUBSUBTYPES_HPP
#define INT2_DDS_GENERATED__DDS_TEST_PUBSUBTYPES_HPP

#include <int2dds/dds/core/policy/QosPolicies.hpp>
#include <int2dds/dds/topic/TopicDataType.hpp>
#include <int2dds/rtps/common/InstanceHandle.hpp>
#include <int2dds/rtps/common/SerializedPayload.hpp>
#include <int2dds/utils/md5.hpp>

#include "dds_Test.hpp"


#if !defined(INT2DDS_GEN_API_VER) || (INT2DDS_GEN_API_VER != 3)
#error \
    Generated dds_Test is not compatible with current installed Int2 DDS. Please, regenerate it with int2ddsgen.
#endif  // INT2DDS_GEN_API_VER


/*!
 * @brief This class represents the TopicDataType of the type Image defined by the user in the IDL file.
 * @ingroup dds_Test
 */
class ImagePubSubType : public intellectus::int2dds::dds::TopicDataType
{
public:

    typedef Image type;

    Intellectus_user_DllExport ImagePubSubType();

    Intellectus_user_DllExport ~ImagePubSubType() override;

    Intellectus_user_DllExport bool serialize(
            const void* const data,
            intellectus::int2dds::rtps::SerializedPayload_t& payload,
            intellectus::int2dds::dds::DataRepresentationId_t data_representation) override;

    Intellectus_user_DllExport bool deserialize(
            intellectus::int2dds::rtps::SerializedPayload_t& payload,
            void* data) override;

    Intellectus_user_DllExport uint32_t calculate_serialized_size(
            const void* const data,
            intellectus::int2dds::dds::DataRepresentationId_t data_representation) override;

    Intellectus_user_DllExport bool compute_key(
            intellectus::int2dds::rtps::SerializedPayload_t& payload,
            intellectus::int2dds::rtps::InstanceHandle_t& ihandle,
            bool force_md5 = false) override;

    Intellectus_user_DllExport bool compute_key(
            const void* const data,
            intellectus::int2dds::rtps::InstanceHandle_t& ihandle,
            bool force_md5 = false) override;

    Intellectus_user_DllExport void* create_data() override;

    Intellectus_user_DllExport void delete_data(
            void* data) override;

    //Register TypeObject representation in Int2 DDS TypeObjectRegistry
    Intellectus_user_DllExport void register_type_object_representation() override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    Intellectus_user_DllExport inline bool is_bounded() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

    Intellectus_user_DllExport inline bool is_plain(
            intellectus::int2dds::dds::DataRepresentationId_t data_representation) const override
    {
        static_cast<void>(data_representation);
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    Intellectus_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        static_cast<void>(memory);
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

private:

    intellectus::int2dds::MD5 md5_;
    unsigned char* key_buffer_;

};

#endif // INT2_DDS_GENERATED__DDS_TEST_PUBSUBTYPES_HPP

