#include "spch.h"
#include "Json.h"
#include <json.hpp>
#include <fstream>
#include <iostream>

namespace Six {

    SIXAPI Json SLoadJS(const SPath& path) {
        Json js;
        std::ifstream is(path.toAbs());
        if (!is.is_open()) {
            std::cerr << "[JsonUtils] Failed to open for reading: " << path.ToString() << std::endl;
            return js;
        }

        try {
            is >> js;
        }
        catch (const Json::parse_error& e) {
            std::cerr << "[JsonUtils] Parse error in " << path.ToString() << ": " << e.what() << std::endl;
        }
        return js;
    }
    SIXAPI bool SLoadJS(const SPath& path, Json& out) {
        std::ifstream is(path.toAbs());
        if (!is.is_open()) {
            std::cerr << "[JsonUtils] Failed to open for reading: " << path.ToString() << std::endl;
            return false;
        }

        try {
            is >> out;
            return true;
        }
        catch (const Json::parse_error& e) {
            std::cerr << "[JsonUtils] Parse error in " << path.ToString() << ": " << e.what() << std::endl;
            return false;
        }
    }

    SIXAPI Json SLoadJS(SPath& path) {
        Json js;
        std::ifstream is(path.toAbs());
        if (!is.is_open()) {
            std::cerr << "[JsonUtils] Failed to open for reading: " << path.ToString() << std::endl;
            return js;
        }

        try {
            is >> js;
        }
        catch (const Json::parse_error& e) {
            std::cerr << "[JsonUtils] Parse error in " << path.ToString() << ": " << e.what() << std::endl;
        }
        return js;
    }
    SIXAPI bool SLoadJS(SPath& path, Json& out) {
        std::ifstream is(path.toAbs());
        if (!is.is_open()) {
            std::cerr << "[JsonUtils] Failed to open for reading: " << path.ToString() << std::endl;
            return false;
        }

        try {
            is >> out;
            return true;
        }
        catch (const Json::parse_error& e) {
            std::cerr << "[JsonUtils] Parse error in " << path.ToString() << ": " << e.what() << std::endl;
            return false;
        }
    }

    SIXAPI std::string SDumpJS(const Json& in) {
        try {
            return in.dump(4);
        }
        catch (const Json::exception& e) {
            std::cerr << "[JsonUtils] Dump error: " << e.what() << std::endl;
            return "{}";
        }
    }

    SIXAPI bool SDumpJS(const SPath& path, const Json& in) {
        std::ofstream file(path.toAbs());
        if (!file.is_open()) {
            std::cerr << "[JsonUtils] Failed to open for writing: " << path.ToString() << std::endl;
            return false;
        }

        try {
            file << in.dump(4);
            return true;
        }
        catch (const Json::exception& e) {
            std::cerr << "[JsonUtils] Dump error to file " << path.ToString() << ": " << e.what() << std::endl;
            return false;
        }
    }
}