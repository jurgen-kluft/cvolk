#include "cunittest/cunittest.h"

#include "cvolk/volk.h"

#define ArraySize(x) (sizeof(x) / sizeof(x[0]))

UNITTEST_SUITE_BEGIN(test_volk)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(init1)
        {
            /* This won't compile if the appropriate Vulkan platform define isn't set. */
            void* ptr =
#if defined(_WIN32)
              &vkCreateWin32SurfaceKHR;
#elif defined(__linux__) || defined(__unix__)
              &vkCreateXlibSurfaceKHR;
#elif defined(__APPLE__)
              &vkCreateMacOSSurfaceMVK;
#else
              /* Platform not recogized for testing. */
              NULL;
#endif
        }

        UNITTEST_TEST(init2)
        {
            const VkResult res = volkInitialize();
            CHECK_EQUAL_T(VK_SUCCESS, res, "Volk Cannot be initialized");

            uint32_t version = volkGetInstanceVersion();

            CHECK_EQUAL(1, VK_VERSION_MAJOR(version));
            CHECK_EQUAL(3, VK_VERSION_MINOR(version));
            CHECK_TRUE(283 <= VK_VERSION_PATCH(version));

            // https://vulkan.lunarg.com/doc/view/1.1.108.0/windows/validation_layers.html
            const char* vLayers[] = {"VK_LAYER_KHRONOS_validation"};

            // uint32_t                 glfwExtensionCount;
            // auto                     glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            // const char* extensions[] = (glfwExtensions, glfwExtensions + glfwExtensionCount);

            const char* extensions[] = {
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
                VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
                VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
            };

            const VkApplicationInfo appInfo = {.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                               .pNext              = nullptr,
                                               .pApplicationName   = "Hello Volk",
                                               .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                                               .pEngineName        = "Hello Engine using Volk",
                                               .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
                                               .apiVersion         = VK_API_VERSION_1_3};

            const VkInstanceCreateInfo createInfo = {.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                                     .pNext                   = nullptr,
                                                     .flags                   = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
                                                     .pApplicationInfo        = &appInfo,
                                                     .enabledLayerCount       = ArraySize(vLayers),
                                                     .ppEnabledLayerNames     = vLayers,
                                                     .enabledExtensionCount   = ArraySize(extensions),
                                                     .ppEnabledExtensionNames = extensions};

            VkInstance instance_;
            VkResult   result = vkCreateInstance(&createInfo, nullptr, &instance_);
            CHECK_EQUAL_T(VK_SUCCESS, result, "Volk Cannot create Vulkan instance")
            if (result != VK_SUCCESS)
            {
                CHECK_NOT_EQUAL_T(VK_ERROR_OUT_OF_HOST_MEMORY, result, "VK_ERROR_OUT_OF_HOST_MEMORY");
                CHECK_NOT_EQUAL_T(VK_ERROR_OUT_OF_DEVICE_MEMORY, result, "VK_ERROR_OUT_OF_DEVICE_MEMORY");
                CHECK_NOT_EQUAL_T(VK_ERROR_INITIALIZATION_FAILED, result, "VK_ERROR_INITIALIZATION_FAILED");
                CHECK_NOT_EQUAL_T(VK_ERROR_LAYER_NOT_PRESENT, result, "VK_ERROR_LAYER_NOT_PRESENT");
                CHECK_NOT_EQUAL_T(VK_ERROR_EXTENSION_NOT_PRESENT, result, "VK_ERROR_EXTENSION_NOT_PRESENT");
                CHECK_NOT_EQUAL_T(VK_ERROR_INCOMPATIBLE_DRIVER, result, "VK_ERROR_INCOMPATIBLE_DRIVER");
            }

            // We use Volk to obtain a Vulkan instance
            volkLoadInstance(instance_);

            // And we immediately unload it
            vkDestroyInstance(instance_, nullptr);
        }
    }
}
UNITTEST_SUITE_END
