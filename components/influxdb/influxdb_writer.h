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
                void set_tags(std::string tags) { this->tags = tags; }
                void set_sensor_names(std::string sensor_names) { this->sensor_names = sensor_names; }

                void on_sensor_update(sensor::Sensor *obj, float state);
#ifdef USE_BINARY_SENSOR
                void on_sensor_update(binary_sensor::BinarySensor *obj, bool state);
#endif
#ifdef USE_TEXT_SENSOR
                void on_sensor_update(text_sensor::TextSensor *obj, std::string state);
#endif

            protected:
                std::string url;
                std::string token;
                std::string org;
                std::string bucket;
                std::string measurement;
                std::string tags;
                /** sensor,binary,text  names  =
                 * "nam1,nam 2,nn nn3" 
                 * 
                 * 
                 */
                std::string sensor_names; 

                std::unique_ptr<InfluxDBClient> client;
                std::unique_ptr<Point> point;
        };
    }
}
