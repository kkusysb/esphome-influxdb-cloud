import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.const import CONF_ID
from esphome.core import coroutine_with_priority, CORE

DEPENDENCIES = ['network']

influxdb_ns = cg.esphome_ns.namespace('influxdb')
InfluxDBWriter = influxdb_ns.class_('InfluxDBWriter', cg.Component, cg.Controller)

CONF_INFLUXDB_URL = 'url'
CONF_INFLUXDB_TOKEN = 'token'
CONF_INFLUXDB_ORG = 'org'
CONF_INFLUXDB_BUCKET = 'bucket'
CONF_INFLUXDB_MEASUREMENT = 'measurement'

# tags : "device=esp71,location=kotlownia"
CONF_INFLUXDB_TAGS = 'tags'
# // sensor_names : "nazaw 1,nazwa2,guzik"
CONF_INFLUXDB_SENSOR_NAMES = 'sensor_names'

CONFIG_SCHEMA = cv.All(
    cv.Schema({
        cv.GenerateID(): cv.declare_id(InfluxDBWriter),
        cv.Required(CONF_INFLUXDB_URL): cv.string,
        cv.Required(CONF_INFLUXDB_TOKEN): cv.string,
        cv.Required(CONF_INFLUXDB_ORG): cv.string,
        cv.Required(CONF_INFLUXDB_BUCKET): cv.string,
        cv.Required(CONF_INFLUXDB_MEASUREMENT): cv.string,
        cv.Required(CONF_INFLUXDB_TAGS): cv.string,
        cv.Required(CONF_INFLUXDB_SENSOR_NAMES): cv.string,

    }).extend(cv.COMPONENT_SCHEMA),
    cv.only_with_arduino,
    cv.only_on(['esp32', 'esp8266']),
)

@coroutine_with_priority(40.0)
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    cg.add(var.set_url(config[CONF_INFLUXDB_URL]))
    cg.add(var.set_token(config[CONF_INFLUXDB_TOKEN]))
    cg.add(var.set_org(config[CONF_INFLUXDB_ORG]))
    cg.add(var.set_bucket(config[CONF_INFLUXDB_BUCKET]))
    cg.add(var.set_measurement(config[CONF_INFLUXDB_MEASUREMENT]))
    cg.add(var.set_tags(config[CONF_INFLUXDB_TAGS]))
    cg.add(var.set_sensor_names(config[CONF_INFLUXDB_SENSOR_NAMES]))
    
    if CORE.is_esp32:
        cg.add_library('WiFiClientSecure', None)
        cg.add_library('HTTPClient', None)
    if CORE.is_esp8266:
        cg.add_library('ESP8266HTTPClient', None)
    
    cg.add_library('tobiasschuerg/ESP8266 Influxdb', '3.13.1')
