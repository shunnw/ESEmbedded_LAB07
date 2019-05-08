#include <stdint.h>
#include "reg.h"
#include "blink.h"

int main(void)
{
	//RCC Enable GPIOA
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTA));

	//GPIO Configurations
	//MODER led pin = 00 => Input (reset state)
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_1_BIT(PUSH_BUTTON_USER_B1));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_0_BIT(PUSH_BUTTON_USER_B1));

	//OSPEEDR led pin = 00 => Low speed
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(PUSH_BUTTON_USER_B1));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(PUSH_BUTTON_USER_B1));

	//PUPDR led pin = 00 => No pull-up, pull-down
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(PUSH_BUTTON_USER_B1));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(PUSH_BUTTON_USER_B1));


	//RCC Enable SYSCFG
	CLEAR_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2_1_BIT);
	CLEAR_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2_0_BIT);

	//SYSCFG Configurations
	WRITE_BITS(SYSCFG_BASE + SYSCFG_EXTICR1_OFFSET, EXTI0_3_BIT, EXTI0_0_BIT, 0b00); // Select source input = 0b00(PA[x] pin) for EXTI0

	//EXTI Configurations
	SET_BIT(EXTI_BASE + EXTI_IMR_OFFSET, 0); // Interrupt request fomr line 0 NOT masked
	CLEAR_BIT(EXTI_BASE + EXTI_RTSR_OFFSET, 0); // Disable Rising trigger
	SET_BIT(EXTI_BASE + EXTI_FTSR_OFFSET, 0);   // Enable Falling trigger

	//NVIC Configurations
	SET_BIT(NVIC_ISER_BASE + NVIC_ISERn_OFFSET(0), 6); //IRQ6

	blink(LED_BLUE);
}

void exti0_handler(void)
{
	blink_count(LED_ORANGE, 10);

	//clear pending
	SET_BIT(EXTI_BASE + EXTI_PR_OFFSET, 0);
}
