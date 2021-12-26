#ifndef VK_QUEUE_HPP__
#define VK_QUEUE_HPP__

#include <vulkan/vulkan.h>

#include "vk-command.hpp"
#include "vk-device.hpp"

namespace vk {
    struct Queue {
        VkQueue queue;

        public:
            Queue(vk::Device& device, uint32_t family)
                : queue(device->get_device_queue(family))
            {}

            void
            submit(const vk::CommandBuffer& command) {
                VkSubmitInfo submit{};
                submit.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                submit.commandBufferCount   = 1;
                submit.pCommandBuffers      = &(command.buffer);
                vkQueueSubmit(queue, 1, &submit, VK_NULL_HANDLE);
            }

            void
            wait_idle() {
                vkQueueWaitIdle(queue);
            }
        private:
    };
} // namespace vk
#endif // VK_QUEUE_HPP__
