/****************************************************************
Copyright (C) 2009 Aleksey Myasnikov
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser GPL as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser GPL for more details.
You should have received a copy of the GNU Lesser GPL
in file LICENSE along with this program.  If not, see
<http://www.gnu.org/licenses/>
I draw your attention to the fact that the library InsularGenetica
is registered Russian Agency for Patents and Trademarks (ROSPATENT),
which issued the "Certificate of official registration of computer
programs for № FIXME FIXME FIXME FIXME". A copy of the certificate
of registration is presented in the file CERTIFICATE in the root
of the project. It does not impose on the end developers / users,
no additional restrictions other than those prescribed by GNU Lesser
GPL, while maintaining information about developer this library.
****************************************************************/
/**
 * @file    ICancelService.h
 * @brief   Service of canceling evaluatings
 * @date    17/05/2009
 * @version 3.3
**/
#ifndef CANCEL_SERVICE_INTERFACE
#define CANCEL_SERVICE_INTERFACE

#include "Interface.h"

namespace InsularGenetica
{
    struct ICancelService : public Interface
    {
        /**
         * @brief   This method provides canceling evaluations.
         *          The method may be not safe-thread.
         * @return  cancel status
        **/
        virtual bool isCanceled() = 0;
    };
};
#endif // CANCEL_SERVICE_INTERFACE
