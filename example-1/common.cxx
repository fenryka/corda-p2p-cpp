#include <glib.h>
#include <librdkafka/rdkafkacpp.h>
#include <stdexcept>

static void
load_config_group(
        RdKafka::Conf & conf,
        GKeyFile &key_file,
        const char *group
) {
    g_autoptr(GError) error = nullptr;

    gchar **ptr = g_key_file_get_keys(&key_file, group, nullptr, &error);
    if (error) {
        g_error("%s", error->message);
    }

    while (*ptr) {
        const char *key = *ptr;
        g_autofree gchar *value = g_key_file_get_string(&key_file, group, key, &error);

        if (error) {
            g_error("Reading key: %s", error->message);
        }

        std::string err;
        auto result = conf.set(std::string (key), std::string (value), err);

        if (result != RdKafka::Conf::ConfResult::CONF_OK) {
            throw std::runtime_error (err);
        }

        ptr++;
    }
}
