#include "vk/vk-shader.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>

namespace vk {
namespace shader {
    static std::vector<char>
    read(const std::string& file) {
        std::ifstream handle(file, std::ios::ate | std::ios::binary);
        if (!handle.is_open()) {
            throw std::runtime_error("failed to open file");
        }

        std::size_t size = static_cast<std::size_t>(handle.tellg());
        std::vector<char> xs(size);

        handle.seekg(0);
        handle.read(xs.data(), size);
        handle.close();

        return xs;
    }

    Shader::Shader(const std::string& file, std::shared_ptr<device::Device> device)
        : device_(device)
    {
        std::vector<char> xs = read(file);

        VkShaderModuleCreateInfo info{};
        info.sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        info.codeSize   = xs.size();
        info.pCode      = reinterpret_cast<const uint32_t*>(xs.data());

        shader          = device_->create_shader_module(info);
    }

    Shader::~Shader() {
        device_->destroy_shader_module(shader);
    }
} // namespace shader
} // namespace vk
