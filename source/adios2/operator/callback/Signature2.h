/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * Signature2.h
 *
 *  Created on: Oct 20, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_OPERATOR_CALLBACK_SIGNATURE2_H_
#define ADIOS2_OPERATOR_CALLBACK_SIGNATURE2_H_

#include "adios2/core/Operator.h"

namespace adios2
{
namespace callback
{

class Signature2 : public Operator
{

public:
    Signature2(const std::function<
                   void(void *, const std::string &, const std::string &,
                        const std::string &, const Dims &)> &function,
               const Params &parameters, const bool debugMode);

    ~Signature2() = default;

    void RunCallback2(void *, const std::string &, const std::string &,
                      const std::string &, const Dims &) final;

private:
    std::function<void(void *, const std::string &, const std::string &,
                       const std::string &, const Dims &)>
        m_Function;
};
}
}

#endif /* ADIOS2_OPERATOR_CALLBACK_SIGNATURE2_H_ */
