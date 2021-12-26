#include <iomanip>
#include <iostream>
#include <vk/vk.hpp>

int
main(int argc, char **argv) {
    VkInstance instance = vk::instance::create();

    std::vector<VkPhysicalDevice> devices = vk::instance::get_devices(instance);
    VkPhysicalDevice physical   = vk::device::select(devices);
    uint32_t family             = vk::device::get_queue_index(physical);
    {
        vk::Device device = std::make_shared<vk::device::Device>(physical, family);

        vk::Shader              shader("build/naive-prefix.comp.spv", device);
        vk::DescriptorSetLayout descriptorLayout(device, 1);
        vk::PipelineLayout      layout(device, descriptorLayout);
        vk::Pipeline            pipeline(device, shader, layout);
        vk::Buffer<float>       buffers(device);

        std::size_t count = 8;
        buffers.create(count, family);
        vk::Memory<float> memory(device, physical, buffers);
        memory.bind(buffers);

        std::shared_ptr<vk::DescriptorPool> descriptorPool;
        descriptorPool = std::make_shared<vk::DescriptorPool>(device, buffers.size());

        vk::DescriptorSet descriptorSet(device, descriptorPool, descriptorLayout);
        descriptorSet.update(buffers);

        vk::CommandPool     commandPool(device, family);
        vk::CommandBuffer   commandBuffer(device, commandPool);
        commandBuffer.begin();
        commandBuffer.bind_pipeline(pipeline);
        commandBuffer.bind_descriptor(layout, descriptorSet);
        commandBuffer.dispatch(count);
        commandBuffer.end();

        float * dataset = memory.map();
        dataset[0] = 3;
        dataset[1] = 1;
        dataset[2] = 7;
        dataset[3] = 0;
        dataset[4] = 4;
        dataset[5] = 1;
        dataset[6] = 6;
        dataset[7] = 3;

        for (std::size_t i = 0; i < count; i++) {
            std::cout   << std::setw(2)
                        << dataset[i]
                        << ((i == count - 1) ? "\n" : ", ");
        }
        std::cout << std::flush;

        memory.unmap();
        dataset = nullptr;

        vk::Queue queue(device, family);
        queue.submit(commandBuffer);
        queue.wait_idle();

        dataset  = memory.map();

        for (std::size_t i = 0; i < count; i++) {
            std::cout   << std::setw(2)
                        << dataset[i]
                        << ((i == count - 1) ? "\n" : ", ");
        }
        std::cout << std::flush;
        memory.unmap();
    }
    vk::instance::destroy(instance);

    return 0;
}
