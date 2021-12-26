#ifndef VK_DEVICE_HPP__
#define VK_DEVICE_HPP__

#include <memory>
#include <stdexcept>
#include <vulkan/vulkan.h>
#include <vector>

namespace vk {
namespace device {
    std::vector<VkQueueFamilyProperties>
    get_queues(const VkPhysicalDevice& physical);

    uint32_t
    get_queue_index(const VkPhysicalDevice& physical);

    VkPhysicalDevice
    select(const std::vector<VkPhysicalDevice>& xs);

    uint32_t
    find_memory_type(
            const VkPhysicalDevice& physical
            , uint32_t              filter
            , VkMemoryPropertyFlags properties
            );

    struct Device {
        VkDevice device;

        Device(const VkPhysicalDevice& physical, uint32_t family) {
            float priority = 1.0;
            VkDeviceQueueCreateInfo queue{};
            queue.sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue.queueFamilyIndex  = family;
            queue.queueCount        = 1;
            queue.pQueuePriorities  = &priority;

            VkDeviceCreateInfo info{};
            info.sType                  = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            info.pQueueCreateInfos      = &queue;
            info.queueCreateInfoCount   = 1;
            info.enabledLayerCount      = 0;

            VkResult result = vkCreateDevice(physical, &info, nullptr, &device);
            if (VK_SUCCESS != result) {
                throw std::runtime_error("failed to create logical device");
            }
        }

        Device(const Device& other)             = delete;
        Device& operator=(const Device& other)  = delete;

        ~Device() {
            vkDestroyDevice(device, nullptr);
        }

        VkShaderModule
        create_shader_module(const VkShaderModuleCreateInfo& info) {
            VkShaderModule shader;

            VkResult result = vkCreateShaderModule(device, &info, nullptr, &shader);
            if (VK_SUCCESS != result) {
                throw std::runtime_error("failed to create shader module");
            }
            return shader;
        }

        void
        destroy_shader_module(VkShaderModule& shader) {
            vkDestroyShaderModule(device, shader, nullptr);
        }

        VkDescriptorSetLayout
        create_descriptor_set_layout(const VkDescriptorSetLayoutCreateInfo& info) {
            VkDescriptorSetLayout set;

            VkResult result = vkCreateDescriptorSetLayout(device, &info, nullptr, &set);
            if (VK_SUCCESS != result) {
                throw std::runtime_error("failed to create descriptor set layout");
            }
            return set;
        }

        void
        destroy_descriptor_set_layout(VkDescriptorSetLayout& set) {
            vkDestroyDescriptorSetLayout(device, set, nullptr);
        }

        VkPipelineLayout
        create_pipeline_layout(const VkPipelineLayoutCreateInfo& info) {
            VkPipelineLayout layout;
            VkResult result = vkCreatePipelineLayout(device, &info, nullptr, &layout);
            if (VK_SUCCESS != result) {
                throw std::runtime_error("failed to create pipeline layout");
            }
            return layout;
        }

        void
        destroy_pipeline_layout(VkPipelineLayout& layout) {
            vkDestroyPipelineLayout(device, layout, nullptr);
        }

        VkPipeline
        create_compute_pipeline(VkComputePipelineCreateInfo& info) {
            VkPipeline pipeline;
            VkResult result = vkCreateComputePipelines(
                    device
                    , VK_NULL_HANDLE
                    , 1
                    , &info
                    , nullptr
                    , &pipeline
                    );
            if (VK_SUCCESS != result) {
                throw std::runtime_error("failed to create pipeline");
            }
            return pipeline;
        }

        void
        destroy_pipeline(VkPipeline& pipeline) {
            vkDestroyPipeline(device, pipeline, nullptr);
        }

        VkBuffer
        create_buffer(const VkBufferCreateInfo& info) {
            VkBuffer buffer;

            VkResult result = vkCreateBuffer(device, &info, nullptr, &buffer);
            if (VK_SUCCESS != result) {
                throw std::runtime_error("failed to create buffers");
            }
            return buffer;
        }

        void
        destroy_buffer(VkBuffer& buffer) {
            vkDestroyBuffer(device, buffer, nullptr);
        }

        VkMemoryRequirements
        get_buffer_memory_requirements(const VkBuffer& buffer) {
            VkMemoryRequirements requirements;
            vkGetBufferMemoryRequirements(device, buffer, &requirements);
            return requirements;
        }

        VkDeviceMemory
        allocate_memory(const VkMemoryAllocateInfo& info) {
            VkDeviceMemory memory;
            VkResult result = vkAllocateMemory(device, &info, nullptr, &memory);
            if (VK_SUCCESS != result) {
                throw std::runtime_error("failed to allocate buffer memory");
            }
            return memory;
        }

        void
        free_memory(VkDeviceMemory& memory) {
            vkFreeMemory(device, memory, nullptr);
        }

        template<typename T>
        T *
        map_memory(VkDeviceMemory& memory) {
            T * xs = nullptr;
            VkResult result = vkMapMemory(
                    device
                    , memory
                    , 0
                    , VK_WHOLE_SIZE
                    , 0
                    , reinterpret_cast<void **>(&xs)
                    );
            if (VK_SUCCESS != result) {
                throw std::runtime_error("failed to map memory");
            }
            return xs;
        }

        void
        unmap_memory(VkDeviceMemory& memory) {
            vkUnmapMemory(device, memory);
        }

        void
        bind_buffer(VkBuffer& buffer, VkDeviceMemory& memory, VkDeviceSize offset) {
            VkResult result = vkBindBufferMemory(device, buffer, memory, offset);
            if (VK_SUCCESS != result) {
                throw std::runtime_error("failed to bind buffer memory");
            }
        }

        VkDescriptorPool
        create_descriptor_pool(const VkDescriptorPoolCreateInfo& info) {
            VkDescriptorPool pool;
            VkResult result = vkCreateDescriptorPool(device, &info, nullptr, &pool);
            if (VK_SUCCESS != result) {
                throw std::runtime_error("failed to create descriptor pool");
            }
            return pool;
        }

        void
        destroy_descriptor_pool(VkDescriptorPool& pool) {
            vkDestroyDescriptorPool(device, pool, nullptr);
        }

        VkDescriptorSet
        allocate_descriptor_sets(const VkDescriptorSetAllocateInfo& info) {
            VkDescriptorSet set;
            VkResult result = vkAllocateDescriptorSets(device, &info, &set);
            if (VK_SUCCESS != result) {
                throw std::runtime_error("failed to allocate descriptor sets");
            }
            return set;
        }

        void
        free_descriptor_sets(const VkDescriptorPool& pool, VkDescriptorSet& set) {
            vkFreeDescriptorSets(device, pool, 1, &set);
        }

        void
        update_descriptor_sets(const std::vector<VkWriteDescriptorSet>& xs) {
            uint32_t size = static_cast<uint32_t>(xs.size());
            vkUpdateDescriptorSets(device, size, xs.data(), 0, nullptr);
        }

        VkCommandPool
        create_command_pool(const VkCommandPoolCreateInfo& info) {
            VkCommandPool pool;
            VkResult result = vkCreateCommandPool(device, &info, nullptr, &pool);
            if (VK_SUCCESS != result) {
                throw std::runtime_error("failed to create command pool");
            }
            return pool;
        }

        void
        destroy_command_pool(VkCommandPool& pool) {
            vkDestroyCommandPool(device, pool, nullptr);
        }

        VkCommandBuffer
        allocate_command_buffers(const VkCommandBufferAllocateInfo& info) {
            VkCommandBuffer buffer;

            VkResult result = vkAllocateCommandBuffers(device, &info, &buffer);
            if (VK_SUCCESS != result) {
                throw std::runtime_error("failed to allocate command buffer");
            }
            return buffer;
        }

        VkQueue
        get_device_queue(uint32_t family) {
            VkQueue queue;
            vkGetDeviceQueue(device, family, 0, &queue);
            return queue;
        }
    };
} // namespace device
    using Device = std::shared_ptr<vk::device::Device>;
} // namespace vk

#endif // VK_DEVICE_HPP__
