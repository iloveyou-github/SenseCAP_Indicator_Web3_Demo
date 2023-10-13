#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "bsp_board.h"
#include "lv_port.h"
#include "esp_event.h"
#include "esp_event_base.h"

#include "indicator_model.h"
#include "indicator_view.h"
//#include "indicator_controller.h"

#include "wsiotsdk.h"
#include "ws_mqtt.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_sntp.h"
#include "esp_random.h"

#include "nvs_flash.h"

static const char *TAG = "app_main";

#define VERSION   "v1.0.0"

#define SENSECAP  "\n\
   _____                      _________    ____         \n\
  / ___/___  ____  ________  / ____/   |  / __ \\       \n\
  \\__ \\/ _ \\/ __ \\/ ___/ _ \\/ /   / /| | / /_/ /   \n\
 ___/ /  __/ / / (__  )  __/ /___/ ___ |/ ____/         \n\
/____/\\___/_/ /_/____/\\___/\\____/_/  |_/_/           \n\
--------------------------------------------------------\n\
 Version: %s %s %s\n\
--------------------------------------------------------\n\
"

ESP_EVENT_DEFINE_BASE(VIEW_EVENT_BASE);
esp_event_loop_handle_t view_event_handle;

//*****************************************//
void app_main(void)
{
    ESP_LOGI("", SENSECAP, VERSION, __DATE__, __TIME__);

    time_t now;
    struct tm timeinfo;
    int msg_id;
    
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(bsp_board_init());
    lv_port_init();


    esp_event_loop_args_t view_event_task_args = {
        .queue_size = 10,
        .task_name = "view_event_task",
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = 10240,
        .task_core_id = tskNO_AFFINITY
    };

    ESP_ERROR_CHECK(esp_event_loop_create(&view_event_task_args, &view_event_handle));

    lv_port_sem_take();
    indicator_view_init();
    lv_port_sem_give();

    indicator_model_init();
    indicator_controller_init();

    default_SetSeed(esp_random());
    iotex_wsiotsdk_init(time, iotex_mqtt_pubscription, iotex_mqtt_subscription);

    ws_mqtt_init();
    iotex_upload_data_init();
  
    static char buffer[128];    /* Make sure buffer is enough for `sprintf` */
    while (1) {
        // sprintf(buffer, "   Biggest /     Free /    Total\n"
        //         "\t  DRAM : [%8d / %8d / %8d]\n"
        //         "\t PSRAM : [%8d / %8d / %8d]",
        //         heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
        //         heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
        //         heap_caps_get_total_size(MALLOC_CAP_INTERNAL),
        //         heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM),
        //         heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
        //         heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
        // ESP_LOGI("MEM", "%s", buffer);

        vTaskDelay(pdMS_TO_TICKS(10000));      

        ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());  
        ESP_LOGI(TAG, "[APP] Time: %d ", (uint32_t)time(0));
    }
}




#if 0
void  pseduo_code_when_dev_poweron(void) {

    int old_status = 0;

    wifi_connecting();
    display_screen_of_connect_to_server();         

    bind_flag = get_bind_flag_from_nvs();

    if (bind_flag == 1)
        goto_screen_sensor_data_show();             // normal process
    else
        goto_screen_prompt_user_to_register_at_portal();

    while(1) {

        status = get_status_from_server();

        switch (status)  {
            case 0:
                // No need to do anything
                break;
            case 1:
                if (bind_flag == 1)
                    goto_screen_prompt_user_to_register_at_portal();

                old_status = status;
                break;
            case 2:
                goto_screen_prompt_user_to_confirm_at_device();
                
                old_status = status;
                break;
            case 3:
                if ( old_status != 0 ) {
                    goto_screen_prompt_user_to_register_success();          // 1/2 --> 3
                    delay(2000);
                    goto_screen_sensor_data_show();                         // normal process
                } else {
                    // No need to do anything                               // 0 --> 3
                }
                    
                if (bind_flag == 0) {
                    bind_flag = 1;
                    save_bind_flag_to_nvs();
                }
        
                break;                     
            default:
                break;
        }

        if (status == 3)
            break;
    }
}
#endif