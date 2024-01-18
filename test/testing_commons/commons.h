#ifndef COMMONS_H
#define COMMONS_H

#include <cpu/state.h>

namespace testing_commons {

::testing::AssertionResult assert_state_equals(State expected, State actual);

}

#endif