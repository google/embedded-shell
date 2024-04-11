/**
 * Copyright 2022 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/


#ifndef __MINI_UART_H__
#define __MINI_UART_H__

/**
 * @brief Put a character on mini-uart IO register.
 *
 * @param c byte to be sent out
 */
void muputc(char c);

/**
 * @brief Get a character from mini-uart IO register
 *
 * @return int byte read from the mini-uart IO register
 */
int mugetc(void);

/**
 * @brief Init the uart, set baud etc
 *
 */
void mini_uart_init(void);

#endif