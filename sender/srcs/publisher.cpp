// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/**
 * @file HelloWorldPublisher.cpp
 *
 */

#include "HelloWorldPubSubTypes.h"  // generated from HelloWorld.idl

#include <chrono>
#include <thread>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

using namespace eprosima::fastdds::dds;

class HelloWorldPublisher
{
private:
  HelloWorld hello_;  // generated type from HelloWorld.idl
  DomainParticipant* participant_;
  Publisher* publisher_;
  Topic* topic_;
  DataWriter* writer_;
  TypeSupport type_;

  // Listener class
  class PubListener : public DataWriterListener
  {
  public:

    PubListener()
      : matched_(0)
    {
    }

    ~PubListener() override
    {
    }

    // Callback when publication is matched
    void on_publication_matched(DataWriter*, const PublicationMatchedStatus& info) override
    {
      if (info.current_count_change == 1) {
        matched_ = info.total_count;
        std::cout << "Publisher matched." << std::endl;
      }
      else if (info.current_count_change == -1) {
        matched_ = info.total_count;
        std::cout << "Publisher unmatched." << std::endl;
      }
      else {
        std::cout << info.current_count_change
                << " is not a valid value for PublicationMatchedStatus current count change." << std::endl;
      }
    }

    std::atomic_int matched_;

  } listener_;

public:
  // Constructor: initialize all attributes to default values
  HelloWorldPublisher()
    : participant_(nullptr)
    , publisher_(nullptr)
    , topic_(nullptr)
    , writer_(nullptr)
    , type_(new HelloWorldPubSubType())  // generated type support from HelloWorld.idl
  {
  }

  // Destructor: delete all entities
  virtual ~HelloWorldPublisher()
  {
    if (writer_ != nullptr) {
      publisher_->delete_datawriter(writer_);
    }
    if (publisher_ != nullptr) {
      participant_->delete_publisher(publisher_);
    }
    if (topic_ != nullptr) {
      participant_->delete_topic(topic_);
    }
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
  }

  //!Initialize the publisher
  bool init()
  {
    // Initialize the data type
    hello_.index(0);
    hello_.message("HelloWorld");

    // Create the DomainParticipant
    DomainParticipantQos participantQos;
    participantQos.name("Participant_publisher");
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);
    if (participant_ == nullptr) {
        return false;
    }

    // Register the Type
    type_.register_type(participant_);

    // Create the publications Topic
    topic_ = participant_->create_topic("HelloWorldTopic", type_.get_type_name(), TOPIC_QOS_DEFAULT);
    if (topic_ == nullptr) {
        return false;
    }

    // Create the Publisher
    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
    if (publisher_ == nullptr) {
        return false;
    }

    // Create the DataWriter
    writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT, &listener_);
    if (writer_ == nullptr) {
        return false;
    }
    return true;
  }

  //!Send a publication
  bool publish() {

    if (listener_.matched_ > 0) {
      hello_.index(hello_.index() + 1);   // Increment the index for each message
      writer_->write(&hello_);
      return true;
    }

    return false;
  }

  //!Run the Publisher
  void run(uint32_t samples) {
    uint32_t samples_sent = 0;
    while (samples_sent < samples) {
      if (publish()) {
        // If the message is published successfully, increment the count and print the message details
        samples_sent++;
        std::cout << "Message: " << hello_.message() << " with index: " << hello_.index()
                    << " SENT" << std::endl;
      }
      // Sleep for 1 second before sending the next message
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }
};

int main(
        int argc,
        char** argv)
{
  // Print a message indicating that the publisher is starting
  std::cout << "Starting publisher." << std::endl;
  // Set the number of samples to send
  uint32_t samples = 100;
  // Create a publisher object
  HelloWorldPublisher* mypub = new HelloWorldPublisher();
  // Initialize the publisher and run it if initialization is successful
  if(mypub->init()) {
      mypub->run(samples);
  }
  // Delete the publisher object to clean up resources
  delete mypub;

  return 0;
}