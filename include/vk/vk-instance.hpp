#ifndef VK_INSTANCE_HPP
#define VK_INSTANCE_HPP

#include <vulkan/vulkan.h>
#include <vector>

namespace vk {
namespace instance {
    VkInstance create();
    void destroy(VkInstance instance);

    std::vector<VkPhysicalDevice>
    get_devices(const VkInstance& instance);
} // namespace instance
} // namespace vk

#endif // VK_INSTANCE_HPP
