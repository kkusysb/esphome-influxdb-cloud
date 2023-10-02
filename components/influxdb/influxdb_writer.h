#pragma once

#include "esphome/core/component.h"
#include "esphome/core/controller.h"

#include <InfluxDbClient.h>

namespace esphome {
    namespace influxdb {
        class InfluxDBWriter : public Component {
            public:
                InfluxDBWriter() {}
                
                void setup() override;
                float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }
                
                void set_url(std::string url) { this->url = url; }
                void set_token(std::string token) { this->token = token; }
                void set_org(std::string org) { this->org = org; }
                void set_bucket(std::string bucket) { this->bucket = bucket; }
                void set_measurement(std::string measurement) { this->measurement = measurement; }
                void set_location(std::string location) { this->location = location; }
                void set_device(std::string device) { this->device = device; }

                void on_sensor_update(sensor::Sensor *obj, float state);
                
            protected:
                std::string url;
                std::string token;
                std::string org;
                std::string bucket;
                std::string measurement;
                std::string location;
                std::string device;

                std::unique_ptr<InfluxDBClient> client;
                std::unique_ptr<Point> point;
        };
    }
}
