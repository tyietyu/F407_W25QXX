#include "ov7725.h"
#include "ov7725config.h"

#include "usart.h"
#include "sccb.h"
//////////////////////////////////////////////////////////////////////////////////
// ALIENTEK MiniSTM32������
// OV7725 ��������
// ����ԭ��@ALIENTEK
// ������̳:www.openedv.com
// �޸�����:2017/11/1
// �汾��V1.0
//////////////////////////////////////////////////////////////////////////////////

// ��ʼ��OV7725
// ����0:�ɹ�
// ��������ֵ:�������
uint8_t OV7725_Init(void)
{
    uint16_t reg;
    uint16_t i = 0;
    // ����IO

    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin   = PCLK_Pin | WRST_Pin | RRST_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                             PBPin PBPin PBPin PBPin */
    GPIO_InitStruct.Pin  = D0_Pin | D1_Pin | D2_Pin | D3_Pin | D4_Pin | D5_Pin | D6_Pin | D7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PEPin PEPin PEPin */
    GPIO_InitStruct.Pin   = OE_Pin | WEN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    SCCB_Init();                           // ��ʼ��SCCB ��IO��
    if (SCCB_WR_Reg(0x12, 0x80)) return 1; // ��λOV7725
    HAL_Delay(50);
    reg = SCCB_RD_Reg(0X1c); // ��ȡ����ID �߰�λ
    reg <<= 8;
    reg |= SCCB_RD_Reg(0X1d); // ��ȡ����ID �Ͱ�λ
    if (reg != OV7725_MID) {
        printf("MID:%d\r\n", reg);
        return 1;
    }
    reg = SCCB_RD_Reg(0X0a); // ��ȡ����ID �߰�λ
    reg <<= 8;
    reg |= SCCB_RD_Reg(0X0b); // ��ȡ����ID �Ͱ�λ
    if (reg != OV7725_PID) {
        printf("HID:%d\r\n", reg);
        return 2;
    }
    // ��ʼ�� OV7725,����QVGA�ֱ���(320*240)
    for (i = 0; i < sizeof(ov7725_init_reg_tb1) / sizeof(ov7725_init_reg_tb1[0]); i++) {
        SCCB_WR_Reg(ov7725_init_reg_tb1[i][0], ov7725_init_reg_tb1[i][1]);
    }
    return 0x00; // ok
}
////////////////////////////////////////////////////////////////////////////
// OV7725��������
// ��ƽ������
// 0:�Զ�ģʽ
// 1:����
// 2,����
// 3,�칫��
// 4,����
// 5,ҹ��
void OV7725_Light_Mode(uint8_t mode)
{
    switch (mode) {
        case 0:                      // Auto���Զ�ģʽ
            SCCB_WR_Reg(0x13, 0xff); // AWB on
            SCCB_WR_Reg(0x0e, 0x65);
            SCCB_WR_Reg(0x2d, 0x00);
            SCCB_WR_Reg(0x2e, 0x00);
            break;
        case 1:                      // sunny������
            SCCB_WR_Reg(0x13, 0xfd); // AWB off
            SCCB_WR_Reg(0x01, 0x5a);
            SCCB_WR_Reg(0x02, 0x5c);
            SCCB_WR_Reg(0x0e, 0x65);
            SCCB_WR_Reg(0x2d, 0x00);
            SCCB_WR_Reg(0x2e, 0x00);
            break;
        case 2:                      // cloudy������
            SCCB_WR_Reg(0x13, 0xfd); // AWB off
            SCCB_WR_Reg(0x01, 0x58);
            SCCB_WR_Reg(0x02, 0x60);
            SCCB_WR_Reg(0x0e, 0x65);
            SCCB_WR_Reg(0x2d, 0x00);
            SCCB_WR_Reg(0x2e, 0x00);
            break;
        case 3:                      // office���칫��
            SCCB_WR_Reg(0x13, 0xfd); // AWB off
            SCCB_WR_Reg(0x01, 0x84);
            SCCB_WR_Reg(0x02, 0x4c);
            SCCB_WR_Reg(0x0e, 0x65);
            SCCB_WR_Reg(0x2d, 0x00);
            SCCB_WR_Reg(0x2e, 0x00);
            break;
        case 4:                      // home������
            SCCB_WR_Reg(0x13, 0xfd); // AWB off
            SCCB_WR_Reg(0x01, 0x96);
            SCCB_WR_Reg(0x02, 0x40);
            SCCB_WR_Reg(0x0e, 0x65);
            SCCB_WR_Reg(0x2d, 0x00);
            SCCB_WR_Reg(0x2e, 0x00);
            break;

        case 5:                      // night��ҹ��
            SCCB_WR_Reg(0x13, 0xff); // AWB on
            SCCB_WR_Reg(0x0e, 0xe5);
            break;
    }
}
// ɫ������
// sat:-4~+4
void OV7725_Color_Saturation(int8_t sat)
{
    if (sat >= -4 && sat <= 4) {
        SCCB_WR_Reg(USAT, (sat + 4) << 4);
        SCCB_WR_Reg(VSAT, (sat + 4) << 4);
    }
}
// ��������
// bright��-4~+4
void OV7725_Brightness(int8_t bright)
{
    uint8_t bright_value, sign;
    switch (bright) {
        case 4:
            bright_value = 0x48;
            sign         = 0x06;
            break;
        case 3:
            bright_value = 0x38;
            sign         = 0x06;
            break;
        case 2:
            bright_value = 0x28;
            sign         = 0x06;
            break;
        case 1:
            bright_value = 0x18;
            sign         = 0x06;
            break;
        case 0:
            bright_value = 0x08;
            sign         = 0x06;
            break;
        case -1:
            bright_value = 0x08;
            sign         = 0x0e;
            break;
        case -2:
            bright_value = 0x18;
            sign         = 0x0e;
            break;
        case -3:
            bright_value = 0x28;
            sign         = 0x0e;
            break;
        case -4:
            bright_value = 0x38;
            sign         = 0x0e;
            break;
    }
    SCCB_WR_Reg(BRIGHT, bright_value);
    SCCB_WR_Reg(SIGN, sign);
}
// �Աȶ�����
// contrast��-4~+4
void OV7725_Contrast(int8_t contrast)
{
    if (contrast >= -4 && contrast <= 4) {
        SCCB_WR_Reg(CNST, (0x30 - (4 - contrast) * 4));
    }
}
// ��Ч����
// 0:��ͨģʽ
// 1,��Ƭ
// 2,�ڰ�
// 3,ƫ��ɫ
// 4,ƫ��ɫ
// 5,ƫ��ɫ
// 6,����
void OV7725_Special_Effects(uint8_t eft)
{
    switch (eft) {
        case 0: // ����
            SCCB_WR_Reg(0xa6, 0x06);
            SCCB_WR_Reg(0x60, 0x80);
            SCCB_WR_Reg(0x61, 0x80);
            break;
        case 1: // �ڰ�
            SCCB_WR_Reg(0xa6, 0x26);
            SCCB_WR_Reg(0x60, 0x80);
            SCCB_WR_Reg(0x61, 0x80);
            break;
        case 2: // ƫ��
            SCCB_WR_Reg(0xa6, 0x1e);
            SCCB_WR_Reg(0x60, 0xa0);
            SCCB_WR_Reg(0x61, 0x40);
            break;
        case 3: // ����
            SCCB_WR_Reg(0xa6, 0x1e);
            SCCB_WR_Reg(0x60, 0x40);
            SCCB_WR_Reg(0x61, 0xa0);
            break;
        case 4: // ƫ��
            SCCB_WR_Reg(0xa6, 0x1e);
            SCCB_WR_Reg(0x60, 0x80);
            SCCB_WR_Reg(0x61, 0xc0);
            break;
        case 5: // ƫ��
            SCCB_WR_Reg(0xa6, 0x1e);
            SCCB_WR_Reg(0x60, 0x60);
            SCCB_WR_Reg(0x61, 0x60);
            break;
        case 6: // ����
            SCCB_WR_Reg(0xa6, 0x46);
            break;
    }
}
// ����ͼ���������
// width:���ͼ����,<=320
// height:���ͼ��߶�,<=240
// mode:0��QVGA���ģʽ��1��VGA���ģʽ
// QVGAģʽ���ӷ�Χ�㵫���ﲻ�Ǻ�������VGAģʽ���ӷ�ΧС��������
void OV7725_Window_Set(uint16_t width, uint16_t height, uint8_t mode)
{
    uint8_t raw, temp;
    uint16_t sx, sy;

    if (mode) {
        sx = (640 - width) / 2;
        sy = (480 - height) / 2;
        SCCB_WR_Reg(COM7, 0x06);   // ����ΪVGAģʽ
        SCCB_WR_Reg(HSTART, 0x23); // ˮƽ��ʼλ��
        SCCB_WR_Reg(HSIZE, 0xA0);  // ˮƽ�ߴ�
        SCCB_WR_Reg(VSTRT, 0x07);  // ��ֱ��ʼλ��
        SCCB_WR_Reg(VSIZE, 0xF0);  // ��ֱ�ߴ�
        SCCB_WR_Reg(HREF, 0x00);
        SCCB_WR_Reg(HOutSize, 0xA0); // ����ߴ�
        SCCB_WR_Reg(VOutSize, 0xF0); // ����ߴ�
    } else {
        sx = (320 - width) / 2;
        sy = (240 - height) / 2;
        SCCB_WR_Reg(COM7, 0x46);   // ����ΪQVGAģʽ
        SCCB_WR_Reg(HSTART, 0x3f); // ˮƽ��ʼλ��
        SCCB_WR_Reg(HSIZE, 0x50);  // ˮƽ�ߴ�
        SCCB_WR_Reg(VSTRT, 0x03);  // ��ֱ��ʼλ��
        SCCB_WR_Reg(VSIZE, 0x78);  // ��ֱ�ߴ�
        SCCB_WR_Reg(HREF, 0x00);
        SCCB_WR_Reg(HOutSize, 0x50); // ����ߴ�
        SCCB_WR_Reg(VOutSize, 0x78); // ����ߴ�
    }
    raw  = SCCB_RD_Reg(HSTART);
    temp = raw + (sx >> 2); // sx��8λ����HSTART,��2λ����HREF[5:4]
    SCCB_WR_Reg(HSTART, temp);
    SCCB_WR_Reg(HSIZE, width >> 2); // width��8λ����HSIZE,��2λ����HREF[1:0]

    raw  = SCCB_RD_Reg(VSTRT);
    temp = raw + (sy >> 1); // sy��8λ����VSTRT,��1λ����HREF[6]
    SCCB_WR_Reg(VSTRT, temp);
    SCCB_WR_Reg(VSIZE, height >> 1); // height��8λ����VSIZE,��1λ����HREF[2]

    raw  = SCCB_RD_Reg(HREF);
    temp = ((sy & 0x01) << 6) | ((sx & 0x03) << 4) | ((height & 0x01) << 2) | (width & 0x03) | raw;
    SCCB_WR_Reg(HREF, temp);

    SCCB_WR_Reg(HOutSize, width >> 2);
    SCCB_WR_Reg(VOutSize, height >> 1);

    SCCB_RD_Reg(EXHCH);
    temp = (raw | (width & 0x03) | ((height & 0x01) << 2));
    SCCB_WR_Reg(EXHCH, temp);
}
