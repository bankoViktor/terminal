#pragma once

//
//
//static const button_t g_page_input_buttons[BUTTON_COUNT] = {
//    // top
//    SET_BUTTON(button_type_t::Goto, "RTN"),
//    NO_BUTTON,
//    SET_BUTTON(button_type_t::Action, "CLR"),
//    SET_BUTTON(button_type_t::Action, "BCK\nSPC"),
//    SET_BUTTON(button_type_t::Goto, "ENT"),
//    // right
//    SET_BUTTON(button_type_t::Action, "1"),
//    SET_BUTTON(button_type_t::Action, "2"),
//    SET_BUTTON(button_type_t::Action, "3"),
//    SET_BUTTON(button_type_t::Action, "4"),
//    SET_BUTTON(button_type_t::Action, "5"),
//    // bottom
//    SET_BUTTON(button_type_t::Action, "-->"),
//    SET_BUTTON(button_type_t::Action, "<--"),
//    SET_BUTTON(button_type_t::Action, "DOT"),
//    NO_BUTTON,
//    NO_BUTTON,
//    // left
//    SET_BUTTON(button_type_t::Action, "6"),
//    SET_BUTTON(button_type_t::Action, "7"),
//    SET_BUTTON(button_type_t::Action, "8"),
//    SET_BUTTON(button_type_t::Action, "9"),
//    SET_BUTTON(button_type_t::Action, "0"),
//};
//
//
//using page_input_data_t = struct page_input_data_t
//{
//    const char*     szTitle;
//    const char*     szFormat;
//    char            szValue[32];
//    uint32_t        min;
//    uint32_t        max;
//    uint32_t*       pnValue;
//};
//
//
//page_input_data_t g_page_input;
//
//
//template <typename T>
//struct page_input_t
//{
//    const page_info_t<T> operator()()
//    {
//        page_info_t<T> page =
//        {
//            .buttons = g_page_input_buttons,
//            .pdata = &g_page_input,
//            .render = [](terminal_t<T>& terminal, void* pdata)
//            {
//                auto& data = *(page_input_data_t*)pdata;
//
//                rect_t rc;
//                terminal.getClientRect(rc);
//
//                rect_t rcText;
//
//                // Title
//                if (data.szTitle)
//                {
//                    terminal.context.calcText(rcText, data.szTitle,
//                        horizontal_aligment_t::Center, vertical_aligment_t::Top);
//
//                    auto dx = rc.left + (rc.width() - rcText.width()) / 2;
//                    auto dy = rc.top + (coord_t)(rc.height() * .25);
//                    rcText.offset(dx, dy);
//                    terminal.context.text(rcText, data.szTitle, TEXT_COLOR, TEXT_BGCOLOR,
//                        horizontal_aligment_t::Center, vertical_aligment_t::Top);
//                }
//
//                // Value
//                char szBuffer[64] = { 0 };
//                szBuffer[0] = '[';
//                szBuffer[1] = ' ';
//                memset(szBuffer + 2, 'x', 3);
//                //strcpy_s(szBuffer + 1, strlen(data.szValue) + 1, data.szValue);
//                szBuffer[5] = ' ';
//                szBuffer[6] = ']';
//                szBuffer[7] = '\0'; 
//
//                rcText.erase();
//                terminal.context.calcText(rcText, szBuffer,
//                    horizontal_aligment_t::Center, vertical_aligment_t::Top);
//
//                auto dx = rc.left + (rc.width() - rcText.width()) / 2;
//                auto dy = rc.top + (coord_t)(rc.height() * .40);
//                rcText.offset(dx, dy);
//                terminal.context.text(rcText, szBuffer, TEXT_COLOR, TEXT_BGCOLOR,
//                    horizontal_aligment_t::Center, vertical_aligment_t::Top);
//                
//                // Buttons
//                terminal.drawButtons(g_page_input_buttons);
//            },
//            .input = [](terminal_t<T>& terminal, uint8_t index)
//            {
//                switch (index)
//                {
//                case 0:  // RTN
//                    terminal.navigator.pop();
//                    break;
//                case 2:  // CLR
//                    break;
//                case 3:  // BCK SPC
//                    break;
//                case 4:  // ENT
//                {
//                    //char szBuffer[8*4] = { 0 };
//                    //snprintf(szBuffer, 8*4, "", g_page_input.);
//                    *g_page_input.pnValue = 2;// 0x30 + numb;
//
//                    terminal.navigator.pop();
//                }
//                    break;
//                case 5:  // 1
//                case 6:  // 2
//                case 7:  // 3
//                case 8:  // 4
//                case 9:  // 5
//                case 15: // 6
//                case 16: // 7
//                case 17: // 8
//                case 18: // 9
//                case 19: // 0
//                {
//                    auto numb = index == 19 ? 0 : index <= 9 ? index - 4 : index - 9;
//                    auto len = strlen(g_page_input.szValue);
//                    g_page_input.szValue[len] = 0x30 + numb;
//                }
//                    break;
//                case 10: // ->
//                    break;
//                case 11: // <-
//                    break;
//                case 12: // DOT
//                    break;
//                }
//            },
//        };
//        return page;
//    }
//};
