#include "esphome/core/application.h"

#include "influxdb_writer.h"

namespace esphome
{
    namespace influxdb
    {
        static const char *const TAG = "influxdb";

        void InfluxDBWriter::setup()
        {
            ESP_LOGCONFIG(TAG, "Setting up InfluxDB Writer...");

            std::vector<EntityBase *> objs;
            int batchSize = 0;

            for (auto *obj : App.get_sensors())
            {
                // if (obj->is_internal())
                //     continue;

                char *sensor_ids = strdup(this->sensor_ids.c_str());
                char *token = strtok(sensor_ids, ","); // zrobić kopie str !!!

                // loop through the string to extract all other tokens
                while (token != NULL)
                {
                    if (!strcpm(token, obj->get_object_id().c_str())){
                        obj->add_on_state_callback([this, obj](float state)
                                                   { this->on_sensor_update(obj, state); });
                        
                        ESP_LOGI(TAG, "Register sensor to export to influxdb: %s", token);

                        batchSize++;
                        break;
                    }
                    // printf(" %s\n", token); // printing each token
                    token = strtok(NULL, ",");
                }

                free(sensor_ids);

                // auto n= obj->get_object_id();

                // for( auto *id : this-> sensors_ids){

                //     if ( n.compare(id) ==0 ){
                //         obj->add_on_state_callback([this, obj](float state)
                //                                { this->on_sensor_update(obj, state); });

                //         batchSize++;
                //     }
                // }
            }

            this->client = std::unique_ptr<InfluxDBClient>(new InfluxDBClient(this->url.c_str(), this->org.c_str(), this->bucket.c_str(), this->token.c_str()));
            this->client->setInsecure();
            this->client->setWriteOptions(WriteOptions().batchSize(batchSize).bufferSize(batchSize * 5));

            if (this->client->validateConnection())
            {
                ESP_LOGI(TAG, "Connected to InfluxDB, using %d batch size", batchSize);
            }
            else
            {
                ESP_LOGE(TAG, "InfluxDB connection failed: %s", this->client->getLastErrorMessage().c_str());
            }

            this->point = std::unique_ptr<Point>(new Point(this->measurement.c_str()));

            this->point->addTag("location", this->location.c_str());
            this->point->addTag("device", this->device.c_str());
        }

        void InfluxDBWriter::on_sensor_update(sensor::Sensor *obj, float state)
        {
            if (isnan(state))
                return;

            this->point->clearFields();
            this->point->addField(obj->get_object_id().c_str(), state);

            if (!this->client->writePoint(*this->point))
            {
                ESP_LOGE(TAG, "InfluxDB write failed: %s", this->client->getLastErrorMessage().c_str());
            }
        }
    }
}
