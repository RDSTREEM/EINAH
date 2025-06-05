#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <unordered_map>
#include <utils.h>

/**
 * @brief Token types for the Einah language.
 */
enum class TokenType
{
    Tilde,     // '~' statement terminator (like ';')
    Semicolon, // unused, for compatibility
    Identifier,
    Number,
    Equals,       // unused, for compatibility
    Arrow,        // '->' assignment operator (like '=')
    OpenParen,    // '('
    CloseParen,   // ')'
    Plus,         // '+'
    Minus,        // '-'
    Star,         // '*'
    Slash,        // '/'
    Percent,      // '%'
    Eq,           // '~~' equality operator
    Neq,          // '!~' not-equal operator
    And,          // '&=' logical and
    Or,           // '|=' logical or
    Not,          // '~!' logical not
    Sprout,       // 'sprout' mutable variable declaration (like 'let')
    Root,         // 'root' immutable variable declaration (like 'const')
    Boolean,      // 'yup' or 'nope'
    Null,         // 'zip' (null literal)
    Spit,         // 'spit' (print statement)
    Less,         // '<' less than
    Greater,      // '>' greater than
    LessEq,       // '<~' less than or equal
    GreaterEq,    // '>~' greater than or equal
    Whisper,      // 'whisper' conditional
    Then,         // 'then' keyword
    OrKeyword,    // 'or'
    OpenBracket,  // '['
    CloseBracket, // ']'
    String,       // #...# string literal
    EOFTk,        // end of file
    AngleOpen,    // '<' used as a delimiter (not relational)
    AngleClose,   // '>' used as a delimiter (not relational)
    Twirl,        // 'twirl' while loop keyword
    Spin,         // 'spin' while loop block keyword
    Skip,         // 'skip' continue statement
    Shatter,      // 'shatter' break statement
    Dollar,       // '$' for array literal delimiters
    Comma,        // ',' for separating array elements
    Dot,          // '.' for property/array access
    Conjure,      // 'conjure' function definition keyword
    Pipe,         // '|' function call operator
    Return,       // 'zipback' return statement
};

/**
 * @brief Structure representing a token in the Einah language.
 */
struct Token
{
    TokenType type;    // The type of the token.
    std::string value; // The string value of the token.
};

/**
 * @brief Reserved keywords for the Einah language.
 */
const inline std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"sprout", TokenType::Sprout},
    {"root", TokenType::Root},
    {"spit", TokenType::Spit},
    {"zip", TokenType::Null},
    {"whisper", TokenType::Whisper},
    {"then", TokenType::Then},
    {"or", TokenType::OrKeyword},
    {"twirl", TokenType::Twirl},
    {"spin", TokenType::Spin},
    {"skip", TokenType::Skip},
    {"shatter", TokenType::Shatter},
    {"conjure", TokenType::Conjure},
    {"zipback", TokenType::Return},
};

/**
 * @brief Tokenize source code into a vector of tokens.
 * @param sourceCode The source code to tokenize.
 * @return std::vector<Token> The list of tokens.
 */
std::vector<Token> tokenize(const std::string &sourceCode);

/**
 * @brief Creates token of the given type and value
 * @param type The type of token to be created
 * @param value The value of the token
 * @return Token{type, value}
 */
Token createToken(TokenType type, const std::string &value);

#endif