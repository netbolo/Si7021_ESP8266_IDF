
#include "si7021.h"

static const char *TAG = "main";

static void i2c_task_example(void *arg)
{
    uint8_t Si7021_ErrCode;
    uint8_t Current;
    uint8_t HeaterControlReg;
    float Humidity;
    float Temperature;
    uint8_t i;   
    // Si7021_ErrCode = Si7021_WriteUserRegister(0x3A);
    Si7021_ReadFirmwareRev();
    Si7021_ErrCode = Si7021_SetResolution(H11_T11);
    ESP_LOGI(TAG, "Si7021_ErrCode: 0x%0X\n", Si7021_ErrCode);
    HeaterControlReg = Si7021_ReadHeaterCRegister();
    ESP_LOGI(TAG, "Si7021_HeaterCRegisterValue: 0x%0X\n", HeaterControlReg);
    ESP_LOGI(TAG, "Si7021_ErrCode: 0x%0X\n", Si7021_ErrCode);
    Si7021_ErrCode = Si7021_EnableHeater(0x01);
    // Si7021_ErrCode = Si7021_WriteUserRegister(0xBE);
    // Si7021_ErrCode = Si7021_WriteUserRegister(0x3A);
    Si7021_GetResolution();
    while (1)
     {
         for(i = 3; i<90; i++)
         {
         ESP_LOGI(TAG, "Si7021_StepI: %d mA\n", i);
         Si7021_SetHeaterCurrent(i);
         Current = Si7021_ReadHeaterCurrent();
         ESP_LOGI(TAG, "Si7021_HeaterCurrent: %d mA\n", Current);

         Humidity = Si7021_ReadHumidity();
         Temperature = Si7021_ReadTemperature();
	     ESP_LOGI(TAG, "Humidity:%d.%d, Temperature: %d.%d\n",
                        (uint16_t)Humidity,(uint16_t)(Humidity * 100) % 100, 
                        (uint16_t)Temperature, (uint16_t)(Temperature * 100) % 100 );

         vTaskDelay(2000 / portTICK_RATE_MS);
         }
    }

    // i2c_driver_delete(I2C_EXAMPLE_MASTER_NUM);
}

void app_main(void)
{
    Si7021_Init();
    
    //start i2c task
    xTaskCreate(i2c_task_example, "i2c_task_example", 2048, NULL, 10, NULL);
}
