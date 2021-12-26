#include <iostream>
#include <vk/vk.hpp>

int
main(int argc, char **argv) {
    VkInstance instance = vk::instance::create();

    std::vector<VkPhysicalDevice> devices = vk::instance::get_devices(instance);
    VkPhysicalDevice physical   = vk::device::select(devices);
    uint32_t family             = vk::device::get_queue_index(physical);
    VkDevice device             = vk::device::create(physical, family);

    VkShaderModule shader                   = vk::shader::create("build/prefix.comp.spv", device);
    VkDescriptorSetLayout descriptorLayout  = vk::descriptor::layout::create(device, 1);
    VkPipelineLayout layout                 = vk::pipeline::layout::create(device, descriptorLayout);
    VkPipeline pipeline                     = vk::pipeline::create(device, shader, layout);

    vk::pipeline::destroy(device, pipeline);
    vk::pipeline::layout::destroy(device, layout);
    vk::descriptor::layout::destroy(device, descriptorLayout);
    vk::shader::destroy(device, shader);

    vk::device::destroy(device);
    vk::instance::destroy(instance);

    return 0;
}
