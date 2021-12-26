#ifndef VK_MEMORY_HPP__
#define VK_MEMORY_HPP__

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <vector>

#include "vk-buffer.hpp"
#include "vk-device.hpp"

namespace vk {
    template<typename T>
    struct Memory {
        public:
            VkDeviceMemory memory;

            Memory(
                    vk::Device&                 device
                    , const VkPhysicalDevice&   physical
                    , const vk::Buffer<T>&      buffer
                    )
                : device_(device)
            {
                std::vector<VkMemoryRequirements> xs = buffer.get_requirements();

                VkDeviceSize size   = 0;
                uint32_t filter     = 0;

                for (const VkMemoryRequirements& x : xs) {
                    size    += x.size;
                    filter  |= x.memoryTypeBits;
                }
                uint32_t index = vk::device::find_memory_type(
                        physical
                        , filter
                        , VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                        | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
                        );

                VkMemoryAllocateInfo info{};
                info.sType              = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                info.allocationSize     = size;
                info.memoryTypeIndex    = index;

                memory = device_->allocate_memory(info);
            }

            void
            bind(const vk::Buffer<T>& buffers) {
                VkDeviceSize offset = 0;
                std::vector<VkMemoryRequirements> xs = buffers.get_requirements();
                for (std::size_t i = 0; i < xs.size(); i++) {
                    VkMemoryRequirements x  = xs[i];
                    VkBuffer buffer         = buffers.buffers[i];
                    device_->bind_buffer(buffer, memory,  offset);

                    offset += x.size;
                }
            }

            T *
            map() { return device_->map_memory<T>(memory); }

            void
            unmap() { device_->unmap_memory(memory); }

            ~Memory() { device_->free_memory(memory); }
        private:
            vk::Device device_;
    };
} // namespace vk

#endif // VK_MEMORY_HPP__
