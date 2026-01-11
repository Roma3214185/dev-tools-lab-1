#ifndef DATAINPUTSERVICE_H
#define DATAINPUTSERVICE_H

#include <string>

/**
 * @brief Represents the result of a validation procedure.
 *
 * This structure is used by all validation functions to return
 * whether the input value is valid and, if not, a human-readable
 * explanation of the validation error.
 */
struct ValidationResult {
    bool valid; ///< Indicates whether the validation succeeded.
    std::string message; ///< Detailed description of the validation error (empty if valid).
};

/**
 * @brief Configuration parameters used for validating user input.
 *
 * This structure defines all bounds and constraints used by
 * the validation functions. It can be customized to adjust
 * input requirements without changing validation logic.
 */
struct Config {
    size_t kMinLenOfName = 1;     ///< Minimum allowed length of a user name.
    size_t kMaxLenOfName = 64;    ///< Maximum allowed length of a user name.

    size_t kMinPasswordLength = 8;   ///< Minimum allowed password length.
    size_t kMaxPasswordLength = 64;  ///< Maximum allowed password length.

    size_t kMinTagLength = 2;     ///< Minimum allowed tag length.
    size_t kMaxTagLength = 32;    ///< Maximum allowed tag length.

    size_t kMinEmailLocalPartLength = 1;  ///< Minimum length of the email's local part (before domain).
    size_t kMaxEmailLocalPartLength = 64; ///< Maximum length of the email's local part (before domain).
    std::string kEmailDomain = "@gmail.com"; ///< Required email domain.
};

/**
 * @brief Represents user-provided input fields.
 *
 * The validation service checks all the fields in this structure
 * using the rules defined in Config.
 */
struct UserInput {
    std::string name;       ///< The user's name.
    std::string email;      ///< The user's email address.
    std::string password;   ///< The user's password.
    std::string tag;        ///< The user's tag/identifier.
};

/**
 * @namespace DataInputService
 * @brief Contains a set of functions to validate individual pieces of user input.
 *
 * This namespace provides a modular, reusable validation system.
 * All functions return detailed error messages describing
 * exact reasons for failed validation (length mismatch, invalid characters, etc.).
 */
namespace DataInputService {

    /**
     * @brief Validates a user's name and returns a detailed explanation if invalid.
     *
     * Validation includes:
     * - Minimum and maximum length checks.
     * - Potentially additional checks (such as allowed characters),
     *   depending on implementation.
     *
     * @param name The input name string to validate.
     * @param cfg Validation configuration (length constraints).
     * @return ValidationResult Detailed result including validity flag and error message.
     */
    ValidationResult nameValidDetailed(const std::string& name, const Config& cfg = Config());

    /**
     * @brief Validates an email address and returns a detailed explanation if invalid.
     *
     * Validation includes:
     * - Checking the presence of the required domain (`cfg.kEmailDomain`).
     * - Ensuring proper length of the local part.
     * - Ensuring email format rules depending on implementation.
     *
     * @param email The email string to validate.
     * @param cfg Validation configuration (length constraints and domain).
     * @return ValidationResult Detailed validation result.
     */
    ValidationResult emailValidDetailed(const std::string& email, const Config& cfg = Config());

    /**
     * @brief Validates a password according to configuration constraints.
     *
     * Validation includes:
     * - Minimum and maximum length checks.
     * - Additional security rules depending on implementation.
     *
     * @param password The password to validate.
     * @param cfg Validation configuration (password length constraints).
     * @return ValidationResult Detailed validation result.
     */
    ValidationResult passwordValidDetailed(const std::string& password, const Config& cfg = Config());

    /**
     * @brief Validates a user tag and provides a detailed error description.
     *
     * Validation includes:
     * - Length checks.
     * - Additional formatting rules depending on implementation.
     *
     * @param tag The user tag to validate.
     * @param cfg Validation configuration (tag length constraints).
     * @return ValidationResult Detailed validation result.
     */
    ValidationResult tagValidDetailed(const std::string& tag, const Config& cfg = Config());

    /**
     * @brief Validates all fields of a UserInput structure.
     *
     * Validation runs in the following order:
     * 1. nameValidDetailed()
     * 2. emailValidDetailed()
     * 3. passwordValidDetailed()
     * 4. tagValidDetailed()
     *
     * If a field fails validation, no further fields are checked.
     *
     * @param input The structure containing user-supplied name, email, password, and tag.
     * @param cfg Validation configuration.
     * @return ValidationResult The first validation error encountered, or success if all fields are valid.
     */
    ValidationResult validateUserInput(const UserInput& input, const Config& cfg = Config());

} // namespace DataInputService

#endif // DATAINPUTSERVICE_H