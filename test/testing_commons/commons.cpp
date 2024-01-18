#include <cpu/state.h>
#include <gtest/gtest.h>
#include <string>

namespace testing_commons {

::testing::AssertionResult assert_state_equals(State expected, State actual) {
    std::vector<std::string> failures;

    if (expected.instruction_register != actual.instruction_register) {
        failures.push_back("expected instruction_register: " +
                           std::to_string(expected.instruction_register) + " " +
                           "actual instruction_register: " +
                           std::to_string(actual.instruction_register));
    }

    if (expected.program_counter != actual.program_counter) {
        failures.push_back("expected program_counter: " +
                           std::to_string(expected.program_counter) + " " +
                           "actual program_counter: " +
                           std::to_string(actual.program_counter));
    }

    if (expected.microcode_counter != actual.microcode_counter) {
        failures.push_back("expected microcode_counter: " +
                           std::to_string(expected.microcode_counter) + " " +
                           "actual microcode_counter: " +
                           std::to_string(actual.microcode_counter));
    }

    if (expected.a_register != actual.a_register) {
        failures.push_back(
            "expected a_register: " + std::to_string(expected.a_register) +
            " " + "actual a_register: " + std::to_string(actual.a_register));
    }

    if (expected.b_register != actual.b_register) {
        failures.push_back(
            "expected b_register: " + std::to_string(expected.b_register) +
            " " + "actual b_register: " + std::to_string(actual.b_register));
    }

    if (expected.lower_register != actual.lower_register) {
        failures.push_back(
            "expected lower_register: " +
            std::to_string(expected.lower_register) + " " +
            "actual lower_register: " + std::to_string(actual.lower_register));
    }

    if (expected.upper_register != actual.upper_register) {
        failures.push_back(
            "expected upper_register: " +
            std::to_string(expected.upper_register) + " " +
            "actual upper_register: " + std::to_string(actual.upper_register));
    }

    if (expected.flag_register != actual.flag_register) {
        failures.push_back(
            "expected flag_register: " +
            std::to_string(expected.flag_register) + " " +
            "actual flag_register: " + std::to_string(actual.flag_register));
    }

    if (expected.port_0 != actual.port_0) {
        failures.push_back(
            "expected port_0: " + std::to_string(expected.port_0) + " " +
            "actual port_0: " + std::to_string(actual.port_0));
    }

    if (expected.port_1 != actual.port_1) {
        failures.push_back(
            "expected port_1: " + std::to_string(expected.port_1) + " " +
            "actual port_1: " + std::to_string(actual.port_1));
    }

    for (int i = 0; i < cpu::RAM_SIZE; i++) {
        if (expected.ram[i] != actual.ram[i]) {
            failures.push_back("expected ram[" + std::to_string(i) +
                               "]: " + std::to_string(expected.ram[i]) +
                               "actual ram[" + std::to_string(i) +
                               "]: " + std::to_string(actual.ram[i]));
        }
    }

    if (failures.empty()) {
        return ::testing::AssertionSuccess();
    }

    std::string errors;
    for (const std::string &failure : failures) {
        errors += "\n" + failure;
    }
    errors += "\n";
    return ::testing::AssertionFailure() << errors;
}

} // namespace testing_commons