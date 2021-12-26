#ifndef VK_SHADER_HPP__
#define VK_SHADER_HPP__

#include <memory>
#include <string>
#include <vulkan/vulkan.h>

#include "vk-device.hpp"

namespace vk {
namespace shader {
    struct Shader {
        public:
            VkShaderModule shader;

            Shader(const std::string& file, std::shared_ptr<device::Device> device);
            ~Shader();
        private:
            std::shared_ptr<device::Device> device_;
    };
} // namespace shader
    using Shader = vk::shader::Shader;
} // namespace vk

#endif // VK_SHADER_HPP__
