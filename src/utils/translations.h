#pragma once

#include <string>
#include <unordered_map>
#include <string_view>

enum class Language {
    ENGLISH,
    SISWATI
};

class Translations {
public:
    static Translations& instance();
    
    void set_language(Language lang);
    Language get_language() const;
    
    std::string get(std::string_view key) const;
    static std::string t(std::string_view key);

private:
    Translations();
    Language current_language = Language::ENGLISH;
    
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translations;
    void load_translations();
};

#define T(key) Translations::t(key)