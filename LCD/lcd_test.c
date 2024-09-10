uint8_t r = 0;

void demoTFT(void)
{
 ST7735_SetRotation(r);

 ST7735_FillScreen(ST7735_BLACK);

 for(int x = 0; x < ST7735_GetWidth(); x++)
 {
   ST7735_DrawPixel(x, 0, ST7735_WHITE);
   ST7735_DrawPixel(x, ST7735_GetHeight() - 1, ST7735_WHITE);
 }

 for(int y = 0; y < ST7735_GetHeight(); y++)
 {
   ST7735_DrawPixel(0, y, ST7735_WHITE);
   ST7735_DrawPixel(ST7735_GetWidth() - 1, y, ST7735_WHITE);
 }

 ST7735_DrawLine(0, 0, ST7735_GetWidth(), ST7735_GetHeight(), ST7735_WHITE);
 ST7735_DrawLine(ST7735_GetWidth(), 0, 0, ST7735_GetHeight(), ST7735_WHITE);

 HAL_Delay(2000);

 ST7735_FillScreen(ST7735_BLACK);

 for (int i = 0; i < ST7735_GetHeight(); i += 4)
 {
  ST7735_DrawFastHLine(0, i, ST7735_GetWidth() - 1, ST7735_WHITE);
 }

 for (int i = 0; i < ST7735_GetWidth(); i += 4)
 {
  ST7735_DrawFastVLine(i, 0, ST7735_GetHeight() - 1, ST7735_WHITE);
 }

 HAL_Delay(2000);

 // Check fonts
 ST7735_FillScreen(ST7735_BLACK);
 ST7735_DrawString(0, 0, "Font_7x10, red on black, lorem ipsum dolor sit amet", Font_7x10, ST7735_RED, ST7735_BLACK);
 ST7735_DrawString(0, 3*10, "Font_11x18, green, lorem ipsum", Font_11x18, ST7735_GREEN, ST7735_BLACK);
 ST7735_DrawString(0, 3*10+3*18, "Font_16x26", Font_16x26, ST7735_BLUE, ST7735_BLACK);
 HAL_Delay(2000);

 // Check colors
 ST7735_FillScreen(ST7735_BLACK);
 ST7735_DrawString(0, 0, "BLACK", Font_11x18, ST7735_WHITE, ST7735_BLACK);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_BLUE);
 ST7735_DrawString(0, 0, "BLUE", Font_11x18, ST7735_BLACK, ST7735_BLUE);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_RED);
 ST7735_DrawString(0, 0, "RED", Font_11x18, ST7735_BLACK, ST7735_RED);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_GREEN);
 ST7735_DrawString(0, 0, "GREEN", Font_11x18, ST7735_BLACK, ST7735_GREEN);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_CYAN);
 ST7735_DrawString(0, 0, "CYAN", Font_11x18, ST7735_BLACK, ST7735_CYAN);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_MAGENTA);
 ST7735_DrawString(0, 0, "MAGENTA", Font_11x18, ST7735_BLACK, ST7735_MAGENTA);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_YELLOW);
 ST7735_DrawString(0, 0, "YELLOW", Font_11x18, ST7735_BLACK, ST7735_YELLOW);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_WHITE);
 ST7735_DrawString(0, 0, "WHITE", Font_11x18, ST7735_BLACK, ST7735_WHITE);
 HAL_Delay(500);

 // Draw circles
 ST7735_FillScreen(ST7735_BLACK);
 for (int i = 0; i < ST7735_GetHeight() / 2; i += 2)
 {
  ST7735_DrawCircle(ST7735_GetWidth() / 2, ST7735_GetHeight() / 2, i, ST7735_YELLOW);
 }
 HAL_Delay(1000);

 ST7735_FillScreen(ST7735_BLACK);
 ST7735_FillTriangle(0, 0, ST7735_GetWidth() / 2, ST7735_GetHeight(), ST7735_GetWidth(), 0, ST7735_RED);
 HAL_Delay(1000);

 ST7735_FillScreen(ST7735_BLACK);
 ST7735_DrawImage(0, 0, 128, 128, (uint16_t*) test_img_128x128);
 HAL_Delay(3000);

 r++;
}