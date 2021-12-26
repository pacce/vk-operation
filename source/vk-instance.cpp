#include "vk/vk-instance.hpp"

#include <stdexcept>

namespace vk {
namespace instance {
    const std::vector<const char*> validation = { "VK_LAYER_KHRONOS_validation" };

    VkInstance create() {
        VkApplicationInfo app{};
        app.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app.pApplicationName    = "Compute";
        app.applicationVersion  = VK_MAKE_VERSION(0, 1, 0);
        app.pEngineName         = "Engineless";
        app.engineVersion       = VK_MAKE_VERSION(0, 1, 0);
        app.apiVersion          = VK_API_VERSION_1_2;

        VkInstanceCreateInfo info{};
        info.sType                      = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        info.pApplicationInfo           = &app;
        info.enabledExtensionCount      = 0;
        info.ppEnabledExtensionNames    = nullptr;
        info.enabledLayerCount          = static_cast<uint32_t>(validation.size());
        info.ppEnabledLayerNames        = validation.data();
        info.pNext                      = nullptr;

        VkInstance instance{};
        VkResult result = vkCreateInstance(&info, nullptr, &instance);
        if (VK_SUCCESS != result) {
            throw std::runtime_error("failed to create instance");
        }
        return instance;
    }

    void destroy(VkInstance instance) {
        vkDestroyInstance(instance, nullptr);
    }

    std::vector<VkPhysicalDevice>
    get_devices(const VkInstance& instance) {
        uint32_t xlen = 0;
        vkEnumeratePhysicalDevices(instance, &xlen, nullptr);
        if (xlen == 0) {
            throw std::runtime_error("failed to find GPUs with vulkan support");
        }

        std::vector<VkPhysicalDevice> xs(xlen);
        vkEnumeratePhysicalDevices(instance, &xlen, xs.data());
        return xs;
    }
} // namespace instance
} // namespace vk
