/************************************************
*                  Button.h                     *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*             Nikolay(Columbus) Red             *
*                   28.10.2017                  *
*************************************************/

#pragma once

#include <GUI/Widget.h>

namespace C
{

	namespace GUI
	{

		class C_Button : public C_Widget
		{
		public:
			C_Button() {}

			void update() override;
			void draw() override;

			~C_Button() {}
		};

	}

}
