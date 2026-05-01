/*
 ==============================================================================
  STORMWATCH WEATHER MANAGEMENT SYSTEM
  ==============================================================================
  Group Members:
    Muhammad Tahir Hussain  (2025-AI-81)
    Muhammad Anas Alam      (2025-AI-86)
    Amir Hamza              (2025-AI-97)
    Muhammad Abubakar       (2025-AI-98)
  ==============================================================================
  COMPILE (Windows):
    g++ -std=c++17 -o stormwatch WeatherManagementSystem.cpp -lwinhttp
  ==============================================================================
*/

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <cmath>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

// ============================================================================
//  COLORS
// ============================================================================
namespace C
{
    const std::string R = "\033[0m";
    const std::string B = "\033[1m";
    const std::string DIM = "\033[2m";
    const std::string RED = "\033[31m";
    const std::string GRN = "\033[32m";
    const std::string YLW = "\033[33m";
    const std::string BLU = "\033[34m";
    const std::string MAG = "\033[35m";
    const std::string CYN = "\033[36m";
    const std::string WHT = "\033[37m";
    const std::string BRED = "\033[91m";
    const std::string BGRN = "\033[92m";
    const std::string BYLW = "\033[93m";
    const std::string BBLU = "\033[94m";
    const std::string BMAG = "\033[95m";
    const std::string BCYN = "\033[96m";
    const std::string BWHT = "\033[97m";
    const std::string BGRD = "\033[41m";
    const std::string BGBL = "\033[44m";
}

// ============================================================================
//  UI HELPERS
// ============================================================================
namespace UI
{
    void clearScreen() { system("cls"); }

    void hline(char ch = '-', int w = 76, const std::string &col = C::CYN)
    {
        std::cout << col;
        for (int i = 0; i < w; ++i)
            std::cout << ch;
        std::cout << C::R << "\n";
    }

    void dline(int w = 76) { hline('=', w, C::BCYN); }

    void title(const std::string &t, const std::string &col = C::BCYN)
    {
        int w = 76;
        dline(w);
        int pad = (w - 2 - (int)t.size()) / 2;
        int rpad = w - 2 - pad - (int)t.size();
        std::cout << col << C::B << "|";
        for (int i = 0; i < pad; ++i)
            std::cout << " ";
        std::cout << t;
        for (int i = 0; i < rpad; ++i)
            std::cout << " ";
        std::cout << "|" << C::R << "\n";
        dline(w);
    }

    void ok(const std::string &m) { std::cout << C::BGRN << C::B << "  [OK]  " << C::R << m << "\n"; }
    void err(const std::string &m) { std::cout << C::BRED << C::B << " [ERR]  " << C::R << m << "\n"; }
    void wrn(const std::string &m) { std::cout << C::BYLW << C::B << "  [!]   " << C::R << m << "\n"; }
    void inf(const std::string &m) { std::cout << C::BBLU << "  [i]   " << C::R << m << "\n"; }

    std::string input(const std::string &prompt, const std::string &col = C::BCYN)
    {
        std::cout << col << C::B << "  >> " << prompt << C::R << C::CYN << " : " << C::R;
        std::string s;
        std::getline(std::cin, s);
        return s;
    }

    std::string password(const std::string &prompt)
    {
        std::cout << C::BMAG << C::B << "  >> " << prompt << C::R << C::MAG << " : " << C::R;
        std::string s;
        std::getline(std::cin, s);
        return s;
    }

    void pause()
    {
        std::cout << "\n"
                  << C::DIM << "  Press ENTER to continue..." << C::R;
        std::string dummy;
        std::getline(std::cin, dummy);
    }

    int menu(int lo, int hi)
    {
        std::string s;
        while (true)
        {
            std::cout << C::BYLW << C::B << "\n  >> Choice [" << lo << "-" << hi << "]"
                      << C::R << C::YLW << " : " << C::R;
            std::getline(std::cin, s);
            try
            {
                int v = std::stoi(s);
                if (v >= lo && v <= hi)
                    return v;
            }
            catch (...)
            {
            }
            err("Enter a number between " + std::to_string(lo) + " and " + std::to_string(hi));
        }
    }

    void bar(const std::string &label, double pct, int width = 40,
             const std::string &fillCol = C::BGRN)
    {
        int f = std::max(0, std::min(width, (int)(pct / 100.0 * width)));
        std::cout << C::BCYN << "  " << std::setw(20) << std::left << label
                  << C::CYN << "[";
        for (int i = 0; i < width; ++i)
        {
            if (i < f)
                std::cout << fillCol << "#" << C::R;
            else
                std::cout << C::DIM << "." << C::R;
        }
        std::cout << C::CYN << "] " << C::BYLW << C::B
                  << std::fixed << std::setprecision(0) << pct << "%" << C::R << "\n";
    }

    void spinner(const std::string &msg)
    {
        std::cout << C::DIM << "\n  [..] " << msg << C::R << "\n";
    }

    void para(const std::string &text, int maxcol = 70)
    {
        std::istringstream ss(text);
        std::string word;
        int col = 0;
        std::cout << "  ";
        while (ss >> word)
        {
            if (col + (int)word.size() + 1 > maxcol)
            {
                std::cout << "\n  ";
                col = 0;
            }
            std::cout << word << " ";
            col += (int)word.size() + 1;
        }
        std::cout << "\n";
    }

    void kv(const std::string &k, const std::string &v,
            const std::string &kc = C::YLW, const std::string &vc = C::BWHT)
    {
        std::cout << kc << "  " << std::setw(22) << std::left << k << vc << v << C::R << "\n";
    }

    void section(const std::string &s, const std::string &col = C::BCYN)
    {
        std::cout << "\n"
                  << col << C::B << "  [ " << s << " ]" << C::R << "\n";
        hline('-', 76, col);
    }

    void menuBox(const std::string &heading,
                 const std::vector<std::pair<std::string, std::string>> &items,
                 const std::string &col = C::BCYN)
    {
        section(heading, col);
        for (auto &p : items)
        {
            std::cout << col << "   " << C::B << std::setw(5) << std::left << ("[" + p.first + "]")
                      << C::R << C::BWHT << " " << p.second << C::R << "\n";
        }
    }

    std::string badge(const std::string &text, bool good)
    {
        return (good ? C::BGBL + C::BWHT : C::BGRD + C::BWHT) + C::B + " " + text + " " + C::R;
    }
}

// ============================================================================
//  JSON EXTRACTOR
// ============================================================================
class Json
{
public:
    static std::string str(const std::string &j, const std::string &key)
    {
        std::string s = "\"" + key + "\"";
        size_t p = j.find(s);
        if (p == std::string::npos)
            return "";
        p = j.find(':', p);
        if (p == std::string::npos)
            return "";
        p = j.find_first_not_of(" \t\n\r", p + 1);
        if (p == std::string::npos)
            return "";
        if (j[p] == '"')
        {
            size_t a = p + 1, b = j.find('"', a);
            return (b == std::string::npos) ? "" : j.substr(a, b - a);
        }
        size_t e = j.find_first_of(",}\n", p);
        if (e == std::string::npos)
            e = j.size();
        std::string v = j.substr(p, e - p);
        v.erase(std::remove_if(v.begin(), v.end(), ::isspace), v.end());
        return v;
    }

    static double dbl(const std::string &j, const std::string &k)
    {
        std::string v = str(j, k);
        try
        {
            return v.empty() ? 0.0 : std::stod(v);
        }
        catch (...)
        {
            return 0.0;
        }
    }

    static int num(const std::string &j, const std::string &k) { return (int)dbl(j, k); }

    static std::string esc(const std::string &s)
    {
        std::string o;
        for (char c : s)
        {
            if (c == '"')
                o += "\\\"";
            else if (c == '\n')
                o += "\\n";
            else if (c == '\r')
                o += "\\r";
            else if (c == '\\')
                o += "\\\\";
            else
                o += c;
        }
        return o;
    }

    static std::string unesc(const std::string &s)
    {
        std::string o;
        for (size_t i = 0; i < s.size(); ++i)
        {
            if (s[i] == '\\' && i + 1 < s.size())
            {
                ++i;
                if (s[i] == 'n')
                    o += '\n';
                else if (s[i] == '"')
                    o += '"';
                else if (s[i] == '\\')
                    o += '\\';
                else if (s[i] == 't')
                    o += '\t';
                else
                {
                    o += '\\';
                    o += s[i];
                }
            }
            else
                o += s[i];
        }
        return o;
    }

    static std::string firstText(const std::string &resp)
    {
        size_t p = resp.find("\"text\"");
        if (p == std::string::npos)
            p = resp.find("\"content\"");
        if (p == std::string::npos)
            return resp;
        p = resp.find(':', p);
        if (p == std::string::npos)
            return resp;
        p = resp.find('"', p);
        if (p == std::string::npos)
            return resp;
        size_t start = p + 1, end = start;
        while (end < resp.size())
        {
            if (resp[end] == '\\')
            {
                end += 2;
                continue;
            }
            if (resp[end] == '"')
                break;
            ++end;
        }
        return unesc(resp.substr(start, end - start));
    }
};

// ============================================================================
//  HTTP CLIENT  (Windows WinHTTP only)
// ============================================================================
class Http
{
    struct URL
    {
        std::wstring host, path;
        bool https;
    };

    static URL parse(const std::string &url)
    {
        URL u;
        u.https = true;
        std::string s = url;
        if (s.substr(0, 8) == "https://")
            s = s.substr(8);
        else if (s.substr(0, 7) == "http://")
        {
            s = s.substr(7);
            u.https = false;
        }
        auto sl = s.find('/');
        std::string ha = (sl != std::string::npos) ? s.substr(0, sl) : s;
        std::string pa = (sl != std::string::npos) ? s.substr(sl) : "/";
        u.host = std::wstring(ha.begin(), ha.end());
        u.path = std::wstring(pa.begin(), pa.end());
        return u;
    }

    static std::string winReq(const std::string &method, const std::string &url,
                              const std::string &body, const std::vector<std::string> &hdrs)
    {
        auto u = parse(url);
        HINTERNET hS = WinHttpOpen(L"StormWatch/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                   WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hS)
            return "";
        INTERNET_PORT port = u.https ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT;
        HINTERNET hC = WinHttpConnect(hS, u.host.c_str(), port, 0);
        if (!hC)
        {
            WinHttpCloseHandle(hS);
            return "";
        }
        std::wstring wm(method.begin(), method.end());
        HINTERNET hR = WinHttpOpenRequest(hC, wm.c_str(), u.path.c_str(), nullptr,
                                          WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
                                          u.https ? WINHTTP_FLAG_SECURE : 0);
        if (!hR)
        {
            WinHttpCloseHandle(hC);
            WinHttpCloseHandle(hS);
            return "";
        }
        for (auto &h : hdrs)
        {
            std::wstring wh(h.begin(), h.end());
            WinHttpAddRequestHeaders(hR, wh.c_str(), (ULONG)-1L, WINHTTP_ADDREQ_FLAG_ADD);
        }
        LPVOID bd = body.empty() ? WINHTTP_NO_REQUEST_DATA : (LPVOID)body.c_str();
        if (!WinHttpSendRequest(hR, WINHTTP_NO_ADDITIONAL_HEADERS, 0, bd,
                                (DWORD)body.size(), (DWORD)body.size(), 0) ||
            !WinHttpReceiveResponse(hR, nullptr))
        {
            WinHttpCloseHandle(hR);
            WinHttpCloseHandle(hC);
            WinHttpCloseHandle(hS);
            return "";
        }
        std::string resp;
        DWORD av = 0;
        while (WinHttpQueryDataAvailable(hR, &av) && av)
        {
            std::vector<char> buf(av + 1, 0);
            DWORD rd = 0;
            WinHttpReadData(hR, buf.data(), av, &rd);
            resp.append(buf.data(), rd);
        }
        WinHttpCloseHandle(hR);
        WinHttpCloseHandle(hC);
        WinHttpCloseHandle(hS);
        return resp;
    }

public:
    static std::string get(const std::string &url)
    {
        return winReq("GET", url, "", {});
    }

    static std::string post(const std::string &url, const std::string &body,
                            const std::vector<std::string> &hdrs)
    {
        std::vector<std::string> allH = {"Content-Type: application/json"};
        for (auto &h : hdrs)
            if (h.find("Content-Type") == std::string::npos)
                allH.push_back(h);
        return winReq("POST", url, body, allH);
    }
};

// ============================================================================
//  UNIVERSAL AI CHATBOT
//  Key prefix -> provider:
//    gsk_...    -> Groq
//    AIza...    -> Google Gemini
//    sk-ant-... -> Anthropic
//    sk-...     -> OpenAI
//    other+URL  -> Custom OpenAI-compatible
// ============================================================================
enum class AiProv
{
    GROQ,
    GEMINI,
    ANTHROPIC,
    OPENAI,
    CUSTOM,
    NONE
};

class Chatbot
{
    std::string apiKey_, url_;
    AiProv prov_;

    AiProv detect(const std::string &k)
    {
        if (k.empty())
            return AiProv::NONE;
        if (k.size() >= 4 && k.substr(0, 4) == "gsk_")
            return AiProv::GROQ;
        if (k.size() >= 4 && k.substr(0, 4) == "AIza")
            return AiProv::GEMINI;
        if (k.size() >= 7 && k.substr(0, 7) == "sk-ant")
            return AiProv::ANTHROPIC;
        if (k.size() >= 3 && k.substr(0, 3) == "sk-")
            return AiProv::OPENAI;
        return AiProv::CUSTOM;
    }

    std::string oaiBody(const std::string &sys, const std::string &usr, const std::string &model)
    {
        return "{\"model\":\"" + model + "\","
                                         "\"max_tokens\":600,"
                                         "\"messages\":["
                                         "{\"role\":\"system\",\"content\":\"" +
               Json::esc(sys) + "\"},"
                                "{\"role\":\"user\",\"content\":\"" +
               Json::esc(usr) + "\"}"
                                "]}";
    }

    std::string antBody(const std::string &sys, const std::string &usr)
    {
        return "{\"model\":\"claude-haiku-4-5-20251001\","
               "\"max_tokens\":600,"
               "\"system\":\"" +
               Json::esc(sys) + "\","
                                "\"messages\":[{\"role\":\"user\",\"content\":\"" +
               Json::esc(usr) + "\"}]}";
    }

    std::string gemBody(const std::string &sys, const std::string &usr)
    {
        return "{\"system_instruction\":{\"parts\":[{\"text\":\"" + Json::esc(sys) + "\"}]},"
                                                                                     "\"contents\":[{\"parts\":[{\"text\":\"" +
               Json::esc(usr) + "\"}]}]}";
    }

    std::string mock(const std::string &q)
    {
        std::string l = q;
        std::transform(l.begin(), l.end(), l.begin(), ::tolower);
        if (l.find("rain") != std::string::npos)
            return "Moderate rain chance in next 24h. Carry an umbrella as a precaution.";
        if (l.find("hot") != std::string::npos)
            return "High temperatures expected. Stay hydrated and avoid midday sun.";
        if (l.find("storm") != std::string::npos)
            return "Thunderstorm possible. Stay indoors, away from tall trees.";
        return "Conditions are currently stable. Ask about forecasts, safety, or travel advice.";
    }

public:
    Chatbot() : prov_(AiProv::NONE) {}

    void configure(const std::string &key, const std::string &customUrl = "")
    {
        apiKey_ = key;
        url_ = customUrl;
        prov_ = detect(key);
        if (prov_ == AiProv::CUSTOM && customUrl.empty())
            prov_ = AiProv::NONE;
    }

    const std::string &key() const { return apiKey_; }
    const std::string &url() const { return url_; }

    std::string provName() const
    {
        switch (prov_)
        {
        case AiProv::GROQ:
            return "Groq";
        case AiProv::GEMINI:
            return "Google Gemini";
        case AiProv::ANTHROPIC:
            return "Anthropic Claude";
        case AiProv::OPENAI:
            return "OpenAI";
        case AiProv::CUSTOM:
            return "Custom";
        default:
            return "DEMO";
        }
    }

    bool isMock() const { return prov_ == AiProv::NONE; }

    std::string ask(const std::string &userMsg)
    {
        std::string sys = "You are a helpful weather assistant. Answer clearly and practically. "
                          "Give safety tips, forecasts, planning advice. Be concise and friendly.";

        if (prov_ == AiProv::NONE)
            return mock(userMsg);

        std::string body, url, resp;
        std::vector<std::string> hdrs;

        switch (prov_)
        {
        case AiProv::GROQ:
            body = oaiBody(sys, userMsg, "llama-3.3-70b-versatile");
            url = "https://api.groq.com/openai/v1/chat/completions";
            hdrs = {"Authorization: Bearer " + apiKey_, "Content-Type: application/json"};
            resp = Http::post(url, body, hdrs);
            return resp.empty() ? "[ERR] No response from Groq." : Json::firstText(resp);

        case AiProv::GEMINI:
            body = gemBody(sys, userMsg);
            url = "https://generativelanguage.googleapis.com/v1beta/models/"
                  "gemini-1.5-flash:generateContent?key=" +
                  apiKey_;
            hdrs = {"Content-Type: application/json"};
            resp = Http::post(url, body, hdrs);
            return resp.empty() ? "[ERR] No response from Gemini." : Json::firstText(resp);

        case AiProv::ANTHROPIC:
            body = antBody(sys, userMsg);
            url = "https://api.anthropic.com/v1/messages";
            hdrs = {"x-api-key: " + apiKey_, "anthropic-version: 2023-06-01", "Content-Type: application/json"};
            resp = Http::post(url, body, hdrs);
            return resp.empty() ? "[ERR] No response from Anthropic." : Json::firstText(resp);

        case AiProv::OPENAI:
            body = oaiBody(sys, userMsg, "gpt-3.5-turbo");
            url = "https://api.openai.com/v1/chat/completions";
            hdrs = {"Authorization: Bearer " + apiKey_, "Content-Type: application/json"};
            resp = Http::post(url, body, hdrs);
            return resp.empty() ? "[ERR] No response from OpenAI." : Json::firstText(resp);

        case AiProv::CUSTOM:
            body = oaiBody(sys, userMsg, "default");
            hdrs = {"Authorization: Bearer " + apiKey_, "Content-Type: application/json"};
            resp = Http::post(url_, body, hdrs);
            return resp.empty() ? "[ERR] No response." : Json::firstText(resp);

        default:
            return mock(userMsg);
        }
    }
};
Chatbot gBot;

// ============================================================================
//  WEATHER DATA & SERVICE
// ============================================================================
struct WData
{
    std::string city, country, cond, desc, rise, set, ts;
    double temp = 0, feels = 0, tmin = 0, tmax = 0, wind = 0, pres = 0, dew = 0;
    int hum = 0, wdeg = 0, vis = 0, cloud = 0;
    bool ok = false;
};

struct FDay
{
    std::string dayName, dateStr, cond, icon;
    double tmin = 0, tmax = 0, wind = 0, rain = 0;
    int hum = 0, uvIndex = 0;
    double pop = 0;
};

struct HourData
{
    std::string timeStr, cond, icon;
    double temp = 0, feelsLike = 0, wind = 0;
    int hum = 0;
    double pop = 0;
};

class WeatherSvc
{
    bool mock_;
    std::string apiKey_;

    std::string nowStr()
    {
        time_t t = time(nullptr);
        char b[32];
        strftime(b, sizeof(b), "%Y-%m-%d %H:%M:%S", localtime(&t));
        return b;
    }

    std::string fmtT(time_t t)
    {
        char b[16];
        strftime(b, sizeof(b), "%H:%M", localtime(&t));
        return b;
    }

    static std::string condIcon(const std::string &cond)
    {
        std::string cl = cond;
        std::transform(cl.begin(), cl.end(), cl.begin(), ::tolower);
        if (cl.find("thunder") != std::string::npos || cl.find("storm") != std::string::npos)
            return "[//]";
        if (cl.find("rain") != std::string::npos || cl.find("rainy") != std::string::npos)
            return "[..]";
        if (cl.find("snow") != std::string::npos)
            return "[**]";
        if (cl.find("fog") != std::string::npos || cl.find("mist") != std::string::npos ||
            cl.find("haze") != std::string::npos)
            return "[~~]";
        if (cl.find("cloud") != std::string::npos)
            return "[cc]";
        if (cl.find("windy") != std::string::npos)
            return "[>>]";
        if (cl.find("clear") != std::string::npos || cl.find("sunny") != std::string::npos)
            return "[ o]";
        return "[ -]";
    }

    WData parse(const std::string &j, const std::string &city)
    {
        WData w;
        if (j.empty())
            return w;
        w.city = city;
        w.country = Json::str(j, "country");
        w.temp = Json::dbl(j, "temp") - 273.15;
        w.feels = Json::dbl(j, "feels_like") - 273.15;
        w.tmin = Json::dbl(j, "temp_min") - 273.15;
        w.tmax = Json::dbl(j, "temp_max") - 273.15;
        w.hum = Json::num(j, "humidity");
        w.pres = Json::dbl(j, "pressure");
        w.wind = Json::dbl(j, "speed");
        w.wdeg = Json::num(j, "deg");
        w.vis = Json::num(j, "visibility");
        w.cloud = Json::num(j, "all");
        w.cond = Json::str(j, "main");
        w.desc = Json::str(j, "description");
        w.ts = nowStr();
        long sr = (long)Json::dbl(j, "sunrise");
        long ss = (long)Json::dbl(j, "sunset");
        if (sr > 0)
            w.rise = fmtT(sr);
        if (ss > 0)
            w.set = fmtT(ss);
        w.dew = w.temp - ((100.0 - w.hum) / 5.0);
        w.ok = true;
        return w;
    }

    WData mockW(const std::string &city)
    {
        WData w;
        w.city = city;
        w.country = "PK";
        w.temp = 28.5 + (rand() % 10 - 5);
        w.feels = w.temp + 2;
        w.tmin = w.temp - 4;
        w.tmax = w.temp + 6;
        w.hum = 55 + rand() % 30;
        w.pres = 1013 + (rand() % 10 - 5);
        w.wind = 3.5 + rand() % 8;
        w.wdeg = rand() % 360;
        w.vis = 8000 + rand() % 4000;
        w.cloud = rand() % 100;
        static const char *cs[] = {"Clear", "Clouds", "Rain", "Haze", "Partly Cloudy"};
        w.cond = cs[rand() % 5];
        w.desc = w.cond + " sky";
        w.dew = w.temp - ((100.0 - w.hum) / 5.0);
        w.rise = "06:15";
        w.set = "19:42";
        w.ts = nowStr();
        w.ok = true;
        return w;
    }

    std::vector<FDay> mockFc(const std::string &)
    {
        static const char *conds[] = {"Sunny", "Partly Cloudy", "Rainy", "Cloudy",
                                      "Clear", "Thunderstorm", "Windy", "Haze"};
        static const char *dayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        static const char *monNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        std::vector<FDay> f;
        time_t now = time(nullptr);
        for (int i = 0; i < 7; ++i)
        {
            time_t t = now + (time_t)i * 86400;
            struct tm *tm_info = localtime(&t);
            FDay fd;
            fd.dayName = dayNames[tm_info->tm_wday];
            char dateBuf[16];
            snprintf(dateBuf, sizeof(dateBuf), "%02d-%s", tm_info->tm_mday, monNames[tm_info->tm_mon]);
            fd.dateStr = dateBuf;
            fd.cond = conds[rand() % 8];
            fd.icon = condIcon(fd.cond);
            fd.tmin = 16.0 + rand() % 10;
            fd.tmax = fd.tmin + 6.0 + rand() % 8;
            fd.hum = 40 + rand() % 45;
            fd.wind = 2.0 + rand() % 14;
            fd.pop = (fd.cond == "Rainy" || fd.cond == "Thunderstorm")
                         ? 60 + rand() % 35
                         : rand() % 30;
            fd.rain = (fd.pop > 50) ? 2.0 + rand() % 20 : 0;
            fd.uvIndex = (fd.cond == "Clear" || fd.cond == "Sunny")
                             ? 6 + rand() % 5
                             : 1 + rand() % 4;
            f.push_back(fd);
        }
        return f;
    }

    std::vector<HourData> mockHourly(const std::string &)
    {
        static const char *conds[] = {"Clear", "Partly Cloudy", "Cloudy", "Rain",
                                      "Thunderstorm", "Haze", "Windy"};
        std::vector<HourData> h;
        time_t now = time(nullptr);
        struct tm *tm_now = localtime(&now);
        double baseTemp = 22.0 + rand() % 10;
        for (int i = 0; i < 24; ++i)
        {
            time_t t = now + (time_t)i * 3600;
            struct tm *ti = localtime(&t);
            char buf[8];
            strftime(buf, sizeof(buf), "%H:%M", ti);
            HourData hd;
            hd.timeStr = buf;
            int hr = (tm_now->tm_hour + i) % 24;
            double off = -5.0 * cos((hr - 14) * 3.14159 / 12.0);
            hd.temp = baseTemp + off + (rand() % 3 - 1);
            hd.feelsLike = hd.temp - 1.5 + (rand() % 3 - 1);
            hd.cond = conds[rand() % 7];
            hd.icon = condIcon(hd.cond);
            hd.hum = 40 + rand() % 45;
            hd.wind = 2.0 + rand() % 12;
            hd.pop = (hd.cond == "Rain" || hd.cond == "Thunderstorm")
                         ? 55 + rand() % 40
                         : rand() % 25;
            h.push_back(hd);
        }
        return h;
    }

public:
    WeatherSvc() : mock_(true) { srand((unsigned)time(nullptr)); }

    void setKey(const std::string &k)
    {
        apiKey_ = k;
        mock_ = k.empty();
    }

    const std::string &key() const { return apiKey_; }

    WData get(const std::string &city)
    {
        if (mock_)
            return mockW(city);
        std::string j = Http::get(
            "https://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey_);
        if (j.empty())
            return mockW(city);
        return parse(j, city);
    }

    std::vector<FDay> fc(const std::string &c) { return mockFc(c); }
    std::vector<HourData> hourly(const std::string &c) { return mockHourly(c); }
    bool isMock() const { return mock_; }
};
WeatherSvc gWx;

// ============================================================================
//  WEATHER DISPLAY
// ============================================================================
namespace WD
{
    void show(const WData &w)
    {
        if (!w.ok)
        {
            UI::err("Weather data unavailable.");
            return;
        }
        UI::dline();
        std::cout << C::BCYN << C::B << "  CURRENT WEATHER -- " << w.city;
        if (!w.country.empty())
            std::cout << ", " << w.country;
        std::cout << C::R << "\n";
        UI::dline();

        std::string cl = w.cond;
        std::transform(cl.begin(), cl.end(), cl.begin(), ::tolower);
        std::string badge, bc;
        if (cl.find("clear") != std::string::npos || cl.find("sunny") != std::string::npos)
        {
            badge = "CLEAR / SUNNY";
            bc = C::BYLW;
        }
        else if (cl.find("cloud") != std::string::npos)
        {
            badge = "CLOUDY";
            bc = C::WHT;
        }
        else if (cl.find("rain") != std::string::npos)
        {
            badge = "RAINY";
            bc = C::BBLU;
        }
        else if (cl.find("storm") != std::string::npos || cl.find("thunder") != std::string::npos)
        {
            badge = "THUNDERSTORM";
            bc = C::BMAG;
        }
        else if (cl.find("snow") != std::string::npos)
        {
            badge = "SNOW";
            bc = C::BCYN;
        }
        else if (cl.find("fog") != std::string::npos || cl.find("mist") != std::string::npos)
        {
            badge = "FOG / MIST";
            bc = C::DIM;
        }
        else
        {
            badge = w.cond;
            bc = C::BWHT;
        }

        std::cout << "\n  " << bc << C::B << "  *** " << badge << " ***  " << C::R << "\n\n";

        std::cout << C::BGBL << C::BWHT << C::B
                  << "  TEMPERATURE   " << std::fixed << std::setprecision(1)
                  << w.temp << " C   (Feels like " << w.feels << " C)" << C::R << "\n";
        std::cout << C::BLU << "  Min: " << C::BBLU << w.tmin << " C"
                  << "   Max: " << C::BRED << w.tmax << " C"
                  << "   Dew: " << C::CYN << w.dew << " C" << C::R << "\n\n";

        UI::bar("Humidity", (double)w.hum, 36, C::BBLU);
        UI::bar("Wind Speed", std::min(w.wind / 30.0 * 100, 100.0), 36, C::BGRN);
        UI::bar("Pressure", std::max(0.0, std::min(100.0, (w.pres - 970) / 70.0 * 100)), 36, C::BMAG);

        UI::section("DETAILS", C::BCYN);
        UI::kv("Wind :", std::to_string((int)w.wind) + " m/s @ " + std::to_string(w.wdeg) + " deg");
        UI::kv("Pressure :", std::to_string((int)w.pres) + " hPa");
        UI::kv("Visibility :", std::to_string(w.vis / 1000) + " km");
        UI::kv("Cloudiness :", std::to_string(w.cloud) + "%");
        UI::kv("Condition :", w.cond + " -- " + w.desc);
        if (!w.rise.empty())
            UI::kv("Sunrise :", w.rise);
        if (!w.set.empty())
            UI::kv("Sunset  :", w.set);
        UI::kv("Updated :", w.ts);
        UI::dline();
    }

    void forecast(const std::vector<FDay> &fc, const std::string &city)
    {
        UI::title("7-DAY FORECAST -- " + city, C::BBLU);
        std::cout << C::B << C::BBLU << std::left
                  << std::setw(5) << "  #"
                  << std::setw(12) << "DAY/DATE"
                  << std::setw(5) << "ICN"
                  << std::setw(16) << "CONDITION"
                  << std::setw(9) << "LO C"
                  << std::setw(9) << "HI C"
                  << std::setw(9) << "HUM%"
                  << std::setw(9) << "WIND"
                  << std::setw(7) << "POP%"
                  << "RAIN\n"
                  << C::R;
        UI::hline('-', 90, C::BLU);

        int idx = 0;
        for (const auto &d : fc)
        {
            ++idx;
            std::string cc, bc;
            std::string cl = d.cond;
            std::transform(cl.begin(), cl.end(), cl.begin(), ::tolower);
            if (cl.find("sunny") != std::string::npos || cl.find("clear") != std::string::npos)
            {
                cc = C::BYLW;
                bc = C::BYLW;
            }
            else if (cl.find("thunder") != std::string::npos || cl.find("storm") != std::string::npos)
            {
                cc = C::BMAG;
                bc = C::BMAG;
            }
            else if (cl.find("rain") != std::string::npos || cl.find("rainy") != std::string::npos)
            {
                cc = C::BBLU;
                bc = C::BBLU;
            }
            else if (cl.find("snow") != std::string::npos)
            {
                cc = C::BCYN;
                bc = C::BCYN;
            }
            else if (cl.find("windy") != std::string::npos)
            {
                cc = C::BGRN;
                bc = C::BGRN;
            }
            else
            {
                cc = C::BWHT;
                bc = C::WHT;
            }

            std::string dayLabel = (idx == 1) ? "TODAY" : d.dayName + " " + d.dateStr;
            std::string numBadge = (idx == 1)
                                       ? C::BGBL + C::BWHT + " " + std::to_string(idx) + " " + C::R
                                       : C::DIM + " " + std::to_string(idx) + " " + C::R;

            std::cout
                << "  " << numBadge << " "
                << bc << C::B << std::setw(11) << std::left << dayLabel << C::R
                << cc << std::setw(5) << d.icon
                << cc << std::setw(16) << d.cond
                << C::BBLU << std::setw(9) << std::fixed << std::setprecision(1) << d.tmin
                << C::BRED << std::setw(9) << d.tmax
                << C::CYN << std::setw(9) << (std::to_string(d.hum) + "%")
                << C::BGRN << std::setw(9) << std::fixed << std::setprecision(1) << d.wind;

            if (d.pop > 60)
                std::cout << C::BRED << std::setw(7) << (std::to_string((int)d.pop) + "%");
            else if (d.pop > 30)
                std::cout << C::BYLW << std::setw(7) << (std::to_string((int)d.pop) + "%");
            else
                std::cout << C::DIM << std::setw(7) << (std::to_string((int)d.pop) + "%");

            if (d.rain > 0)
                std::cout << C::BBLU << std::fixed << std::setprecision(1) << d.rain << " mm";
            else
                std::cout << C::DIM << "  --";

            std::cout << C::DIM << "  UV:" << d.uvIndex << C::R << "\n";

            if (idx < (int)fc.size())
                UI::hline('-', 90, C::DIM);
        }
        UI::hline('=', 90, C::BBLU);
        std::cout << C::DIM
                  << "  ICN: [ o]=Clear  [cc]=Cloudy  [..]=Rain  [//]=Storm  [**]=Snow  [~~]=Fog  [>>]=Windy\n"
                  << "  LO/HI=Min/Max Temp C   HUM=Humidity   WIND=m/s   POP=Precip %   UV=UV Index\n"
                  << C::R << "\n";
    }

    void hourly(const std::vector<HourData> &hrs, const std::string &city)
    {
        UI::title("24-HOUR FORECAST -- " + city, C::BCYN);
        std::cout << C::B << C::BCYN << std::left
                  << std::setw(8) << "  TIME"
                  << std::setw(5) << "ICN"
                  << std::setw(16) << "CONDITION"
                  << std::setw(9) << "TEMP C"
                  << std::setw(11) << "FEELS C"
                  << std::setw(9) << "HUM%"
                  << std::setw(9) << "WIND"
                  << "POP%\n"
                  << C::R;
        UI::hline('-', 76, C::CYN);

        for (int i = 0; i < (int)hrs.size(); ++i)
        {
            const auto &h = hrs[i];
            int hr = std::stoi(h.timeStr.substr(0, 2));
            bool daytime = (hr >= 6 && hr < 20);
            std::string tc = daytime ? C::BYLW : C::BLU;
            std::string rc = daytime ? C::BWHT : C::DIM;

            std::string cl = h.cond;
            std::transform(cl.begin(), cl.end(), cl.begin(), ::tolower);
            std::string cc = C::BWHT;
            if (cl.find("thunder") != std::string::npos)
                cc = C::BMAG;
            else if (cl.find("rain") != std::string::npos)
                cc = C::BBLU;
            else if (cl.find("snow") != std::string::npos)
                cc = C::BCYN;
            else if (cl.find("clear") != std::string::npos || cl.find("sunny") != std::string::npos)
                cc = C::BYLW;
            else if (cl.find("fog") != std::string::npos || cl.find("haze") != std::string::npos)
                cc = C::DIM;

            std::string timeLbl = (i == 0)
                                      ? C::BGBL + C::BWHT + C::B + " NOW  " + C::R
                                      : tc + "  " + h.timeStr;

            std::cout
                << timeLbl << " "
                << cc << std::setw(5) << h.icon
                << rc << std::setw(16) << h.cond
                << tc << std::setw(9) << std::fixed << std::setprecision(1) << h.temp
                << C::WHT << std::setw(11) << h.feelsLike
                << C::CYN << std::setw(9) << (std::to_string(h.hum) + "%")
                << C::BGRN << std::setw(9) << h.wind;

            if (h.pop > 60)
                std::cout << C::BRED << (int)h.pop << "%";
            else if (h.pop > 30)
                std::cout << C::BYLW << (int)h.pop << "%";
            else
                std::cout << C::DIM << (int)h.pop << "%";

            std::cout << C::R << "\n";
        }
        UI::hline('=', 76, C::BCYN);
        std::cout << C::DIM
                  << "  ICN: [ o]=Clear  [cc]=Cloudy  [..]=Rain  [//]=Storm  [~~]=Haze  [>>]=Windy\n"
                  << "  TEMP/FEELS in Celsius   WIND in m/s   POP=Precip Probability\n"
                  << C::R << "\n";
    }
}

// ============================================================================
//  USER / AUTH
// ============================================================================
enum class Role
{
    ADMIN,
    USER
};

static const std::string ADMIN_KEY = "ADMIN-2024";

class User
{
public:
    std::string uname, pass, name, email, dept, loc, lastLogin;
    Role role;
    bool active;
    int logins;

    User() : role(Role::USER), active(true), logins(0) {}
    User(const std::string &u, const std::string &p, const std::string &n,
         const std::string &e, Role r, const std::string &d = "", const std::string &l = "")
        : uname(u), pass(p), name(n), email(e), dept(d), loc(l),
          role(r), active(true), logins(0) {}

    bool auth(const std::string &p) const { return active && pass == p; }

    void recordLogin()
    {
        time_t t = time(nullptr);
        char b[32];
        strftime(b, sizeof(b), "%Y-%m-%d %H:%M:%S", localtime(&t));
        lastLogin = b;
        ++logins;
    }

    std::string roleStr() const { return role == Role::ADMIN ? "ADMINISTRATOR" : "USER"; }
    static std::string r2s(Role r) { return r == Role::ADMIN ? "ADMIN" : "USER"; }
    static Role s2r(const std::string &s) { return (s == "ADMIN") ? Role::ADMIN : Role::USER; }
};

class UserStore
{
    static const std::string PATH;
    static std::string esc(const std::string &s)
    {
        std::string o;
        for (char c : s)
            o += (c == '|' || c == '\n') ? ' ' : c;
        return o;
    }

public:
    static std::vector<User> load()
    {
        std::vector<User> a;
        std::ifstream f(PATH);
        if (!f.is_open())
            return a;
        std::string line;
        while (std::getline(f, line))
        {
            if (line.empty() || line[0] == '#')
                continue;
            std::vector<std::string> fs;
            std::stringstream ss(line);
            std::string t;
            while (std::getline(ss, t, '|'))
                fs.push_back(t);
            if (fs.size() < 10)
                continue;
            User u(fs[0], fs[1], fs[2], fs[3], User::s2r(fs[4]), fs[8], fs[9]);
            u.active = (fs[5] == "1");
            u.logins = std::stoi(fs[6].empty() ? "0" : fs[6]);
            u.lastLogin = fs[7];
            a.push_back(u);
        }
        return a;
    }

    static void save(const std::vector<User> &a)
    {
        std::ofstream f(PATH, std::ios::trunc);
        if (!f.is_open())
        {
            UI::err("Cannot write " + PATH);
            return;
        }
        f << "# StormWatch Users\n# uname|pass|name|email|role|active|logins|lastlogin|dept|loc\n";
        for (const auto &u : a)
            f << esc(u.uname) << "|" << esc(u.pass) << "|" << esc(u.name) << "|"
              << esc(u.email) << "|" << User::r2s(u.role) << "|"
              << (u.active ? "1" : "0") << "|" << u.logins << "|"
              << esc(u.lastLogin) << "|" << esc(u.dept) << "|" << esc(u.loc) << "\n";
    }

    static bool exists(const std::vector<User> &a, const std::string &un)
    {
        for (const auto &u : a)
            if (u.uname == un)
                return true;
        return false;
    }

    static const std::string &path() { return PATH; }
};
const std::string UserStore::PATH = "users.txt";

// ============================================================================
//  CONFIG STORE
// ============================================================================
class ConfigStore
{
    static const std::string PATH;

public:
    static void save(const std::string &wxKey, const std::string &aiKey,
                     const std::string &aiUrl = "")
    {
        std::ofstream f(PATH, std::ios::trunc);
        if (!f.is_open())
        {
            UI::err("Cannot write " + PATH);
            return;
        }
        f << "# StormWatch Config\n";
        f << "wx_key=" << wxKey << "\n";
        f << "ai_key=" << aiKey << "\n";
        f << "ai_url=" << aiUrl << "\n";
    }

    static void load()
    {
        std::ifstream f(PATH);
        if (!f.is_open())
            return;
        std::string line, wxKey, aiKey, aiUrl;
        while (std::getline(f, line))
        {
            if (line.empty() || line[0] == '#')
                continue;
            auto eq = line.find('=');
            if (eq == std::string::npos)
                continue;
            std::string k = line.substr(0, eq);
            std::string v = line.substr(eq + 1);
            if (k == "wx_key")
                wxKey = v;
            else if (k == "ai_key")
                aiKey = v;
            else if (k == "ai_url")
                aiUrl = v;
        }
        if (!wxKey.empty())
            gWx.setKey(wxKey);
        if (!aiKey.empty())
            gBot.configure(aiKey, aiUrl);
    }

    static const std::string &path() { return PATH; }
};
const std::string ConfigStore::PATH = "config.txt";

// ============================================================================
//  ALERTS
// ============================================================================
struct Alert
{
    std::string type, sev, msg, t0, t1, issuedBy, issuedAt;
};

class AlertSys
{
    static const std::string PATH;
    std::vector<Alert> active_, hist_;

    static std::string esc(const std::string &s)
    {
        std::string o;
        for (char c : s)
            o += (c == '|' || c == '\n') ? ' ' : c;
        return o;
    }

public:
    void load()
    {
        active_.clear();
        std::ifstream f(PATH);
        if (!f.is_open())
            return;
        std::string line;
        while (std::getline(f, line))
        {
            if (line.empty() || line[0] == '#')
                continue;
            std::vector<std::string> fs;
            std::stringstream ss(line);
            std::string t;
            while (std::getline(ss, t, '|'))
                fs.push_back(t);
            if (fs.size() < 7)
                continue;
            active_.push_back({fs[0], fs[1], fs[2], fs[3], fs[4], fs[5], fs[6]});
        }
    }

    void save() const
    {
        std::ofstream f(PATH, std::ios::trunc);
        if (!f.is_open())
        {
            UI::err("Cannot write " + PATH);
            return;
        }
        f << "# StormWatch Alerts -- type|sev|msg|t0|t1|issuedBy|issuedAt\n";
        for (const auto &a : active_)
            f << esc(a.type) << "|" << esc(a.sev) << "|" << esc(a.msg) << "|"
              << esc(a.t0) << "|" << esc(a.t1) << "|" << esc(a.issuedBy) << "|"
              << esc(a.issuedAt) << "\n";
    }

    void add(const std::string &tp, const std::string &sv, const std::string &mg,
             const std::string &t0, const std::string &t1, const std::string &by = "SYSTEM")
    {
        time_t now = time(nullptr);
        char buf[32];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        active_.push_back({tp, sv, mg, t0, t1, by, std::string(buf)});
        save();
    }

    void show() const
    {
        if (active_.empty())
        {
            UI::ok("No active alerts.");
            return;
        }
        std::cout << C::B << C::BRED << std::left
                  << "  " << std::setw(4) << "#"
                  << std::setw(12) << "SEVERITY"
                  << std::setw(18) << "TYPE"
                  << std::setw(14) << "ISSUED BY"
                  << "ISSUED AT\n"
                  << C::R;
        UI::hline('-', 76, C::BRED);
        for (int i = 0; i < (int)active_.size(); ++i)
        {
            const auto &a = active_[i];
            std::string c = (a.sev == "EXTREME")    ? C::BGRD + C::BWHT
                            : (a.sev == "SEVERE")   ? C::BRED
                            : (a.sev == "MODERATE") ? C::BYLW
                                                    : C::BGRN;
            std::cout << c << C::B << "  [" << (i + 1) << "] "
                      << std::setw(12) << a.sev
                      << std::setw(18) << a.type
                      << std::setw(14) << a.issuedBy
                      << C::DIM << a.issuedAt << C::R << "\n"
                      << C::WHT << "      MSG  : " << a.msg << "\n"
                      << C::DIM << "      TIME : " << a.t0 << "  ->  " << a.t1 << "\n"
                      << C::R;
            if (i < (int)active_.size() - 1)
                UI::hline('-', 76, C::DIM);
        }
        UI::hline('=', 76, C::BRED);
    }

    void clear(int i)
    {
        if (i >= 0 && i < (int)active_.size())
        {
            hist_.push_back(active_[i]);
            active_.erase(active_.begin() + i);
            save();
        }
    }

    std::vector<Alert> &alerts() { return active_; }
    static const std::string &path() { return PATH; }
};
const std::string AlertSys::PATH = "alerts.txt";

// ============================================================================
//  LOG
// ============================================================================
class Log
{
    struct E
    {
        std::string ts, user, act, det, lvl;
    };
    std::vector<E> es_;

    std::string now()
    {
        time_t t = time(nullptr);
        char b[32];
        strftime(b, sizeof(b), "%Y-%m-%d %H:%M:%S", localtime(&t));
        return b;
    }

public:
    void log(const std::string &u, const std::string &a,
             const std::string &d = "", const std::string &l = "INFO")
    {
        es_.push_back({now(), u, a, d, l});
    }

    void show(int n = 25)
    {
        UI::title("SYSTEM LOG");
        if (es_.empty())
        {
            UI::inf("No entries.");
            return;
        }
        std::cout << C::B << C::CYN << std::left
                  << std::setw(22) << "TIMESTAMP" << std::setw(14) << "USER"
                  << std::setw(10) << "LEVEL" << std::setw(22) << "ACTION"
                  << "DETAIL\n"
                  << C::R;
        UI::hline('-', 90, C::CYN);
        int s = std::max(0, (int)es_.size() - n);
        for (int i = s; i < (int)es_.size(); ++i)
        {
            const auto &e = es_[i];
            std::string lc = (e.lvl == "WARNING")    ? C::BYLW
                             : (e.lvl == "ERROR")    ? C::BRED
                             : (e.lvl == "CRITICAL") ? C::BGRD
                                                     : C::WHT;
            std::cout << C::DIM << std::setw(22) << e.ts
                      << C::GRN << std::setw(14) << e.user
                      << lc << std::setw(10) << e.lvl
                      << C::CYN << std::setw(22) << e.act
                      << C::WHT << e.det << C::R << "\n";
        }
    }

    int count() const { return (int)es_.size(); }
};

// ============================================================================
//  CHATBOT SESSION
// ============================================================================
void chatSession(const User &u)
{
    UI::clearScreen();
    UI::title("AI WEATHER CHATBOT", C::BMAG);
    std::cout << "\n"
              << C::BMAG << "  Hello " << u.name << "!   Provider: "
              << C::BYLW << gBot.provName() << C::R << "\n"
              << C::MAG << "  Ask weather questions. Type 'exit' to return.\n\n"
              << C::R;
    if (gBot.isMock())
        UI::wrn("DEMO mode. Set a real API key via Admin > Configure API Keys.");
    UI::hline();
    while (true)
    {
        std::string q = UI::input("Your question", C::BGRN);
        if (q == "exit" || q == "quit")
            break;
        if (q.empty())
            continue;
        UI::spinner("Thinking...");
        std::string r = gBot.ask(q);
        std::cout << "\n"
                  << C::BMAG << C::B << "  [AI Response]" << C::R << "\n";
        UI::hline('-', 70, C::MAG);
        std::cout << C::WHT;
        UI::para(r, 70);
        std::cout << C::R;
        UI::hline('-', 70, C::MAG);
        std::cout << "\n";
    }
}

// ============================================================================
//  DASHBOARDS
// ============================================================================
class Dashboard
{
protected:
    User &u;
    std::vector<User> &acc;
    AlertSys &al;
    Log &lg;

    void banner(const std::string &role, const std::string &col)
    {
        UI::clearScreen();
        UI::dline();
        std::cout << C::BCYN << C::B << "  STORMWATCH WEATHER MANAGEMENT SYSTEM" << C::R << "\n";
        std::cout << C::DIM << "  User: " << C::BWHT << u.name
                  << C::DIM << "  |  Role: " << col << C::B << role << C::R
                  << C::DIM << "  |  Weather: "
                  << UI::badge(gWx.isMock() ? "DEMO" : "LIVE", !gWx.isMock())
                  << "  |  AI: "
                  << UI::badge(gBot.isMock() ? "DEMO" : "LIVE", !gBot.isMock())
                  << C::R << "\n";
        UI::dline();
    }

public:
    Dashboard(User &us, std::vector<User> &ac, AlertSys &a, Log &l)
        : u(us), acc(ac), al(a), lg(l) {}
    virtual ~Dashboard() = default;
    virtual void run() = 0;
};

class UserDash : public Dashboard
{
public:
    UserDash(User &u, std::vector<User> &a, AlertSys &al, Log &l) : Dashboard(u, a, al, l) {}

    void run() override
    {
        while (true)
        {
            banner("USER", C::BGRN);
            UI::menuBox("USER PORTAL", {{"1", "Check Current Weather"}, {"2", "7-Day Forecast"}, {"3", "24-Hour Forecast"}, {"4", "Active Alerts"}, {"5", "AI Weather Chatbot"}, {"6", "My Profile"}, {"0", "Logout"}}, C::BGRN);
            int ch = UI::menu(0, 6);
            lg.log(u.uname, "MENU", std::to_string(ch));
            if (ch == 0)
                return;

            if (ch == 1)
            {
                UI::clearScreen();
                UI::title("WEATHER LOOKUP", C::BCYN);
                std::string city = UI::input("City Name");
                if (!city.empty())
                {
                    UI::spinner("Fetching...");
                    WD::show(gWx.get(city));
                    lg.log(u.uname, "WX", city);
                }
                UI::pause();
            }
            else if (ch == 2)
            {
                UI::clearScreen();
                UI::title("7-DAY FORECAST", C::BBLU);
                std::string city = UI::input("City Name");
                if (!city.empty())
                {
                    UI::spinner("Fetching forecast...");
                    WD::forecast(gWx.fc(city), city);
                    lg.log(u.uname, "FC_7DAY", city);
                }
                UI::pause();
            }
            else if (ch == 3)
            {
                UI::clearScreen();
                UI::title("24-HOUR FORECAST", C::BCYN);
                std::string city = UI::input("City Name");
                if (!city.empty())
                {
                    UI::spinner("Fetching hourly data...");
                    WD::hourly(gWx.hourly(city), city);
                    lg.log(u.uname, "FC_HOURLY", city);
                }
                UI::pause();
            }
            else if (ch == 4)
            {
                UI::clearScreen();
                UI::title("ACTIVE ALERTS", C::BRED);
                al.show();
                UI::pause();
            }
            else if (ch == 5)
            {
                chatSession(u);
            }
            else if (ch == 6)
            {
                UI::clearScreen();
                UI::title("MY PROFILE", C::BGRN);
                UI::section("ACCOUNT DETAILS", C::BGRN);
                UI::kv("Username    :", u.uname);
                UI::kv("Full Name   :", u.name);
                UI::kv("Email       :", u.email);
                UI::kv("Role        :", u.roleStr());
                UI::kv("Location    :", u.loc.empty() ? "Not set" : u.loc);
                UI::kv("Last Login  :", u.lastLogin.empty() ? "N/A" : u.lastLogin);
                UI::kv("Login Count :", std::to_string(u.logins));
                std::cout << "\n";
                UI::pause();
            }
        }
    }
};

class AdminDash : public Dashboard
{
public:
    AdminDash(User &u, std::vector<User> &a, AlertSys &al, Log &l) : Dashboard(u, a, al, l) {}

    void run() override
    {
        while (true)
        {
            banner("ADMINISTRATOR", C::BRED);
            UI::menuBox("ADMIN CONTROL PANEL", {{"1", "System Log"}, {"2", "Weather Operations"}, {"3", "Alert Management"}, {"4", "System Statistics"}, {"5", "Configure API Keys"}, {"6", "System Status"}, {"0", "Logout"}}, C::BRED);
            int ch = UI::menu(0, 6);
            lg.log(u.uname, "ADMIN", std::to_string(ch));
            if (ch == 0)
                return;

            if (ch == 1)
            {
                UI::clearScreen();
                lg.show(30);
                UI::pause();
            }
            else if (ch == 2)
            {
                weatherOps();
            }
            else if (ch == 3)
            {
                alertMgmt();
            }
            else if (ch == 4)
            {
                stats();
            }
            else if (ch == 5)
            {
                configKeys();
            }
            else if (ch == 6)
            {
                sysStatus();
            }
        }
    }

    void weatherOps()
    {
        UI::clearScreen();
        UI::title("WEATHER OPERATIONS", C::BCYN);
        std::string city = UI::input("City");
        if (!city.empty())
        {
            UI::spinner("Fetching...");
            WD::show(gWx.get(city));
            WD::forecast(gWx.fc(city), city);
            lg.log(u.uname, "WX_OPS", city);
        }
        UI::pause();
    }

    void alertMgmt()
    {
        UI::clearScreen();
        UI::title("ALERT MANAGEMENT", C::BRED);
        al.show();
        UI::menuBox("OPTIONS", {{"1", "Issue Alert"}, {"2", "Clear Alert"}, {"0", "Back"}}, C::BRED);
        int ac = UI::menu(0, 2);
        if (ac == 1)
        {
            al.add(UI::input("Type"), UI::input("Severity"),
                   UI::input("Message"), UI::input("Start"), UI::input("End"), u.uname);
            UI::ok("Alert issued.");
        }
        else if (ac == 2 && !al.alerts().empty())
        {
            al.clear(std::stoi(UI::input("Index (1-based)")) - 1);
            UI::ok("Alert cleared.");
        }
        UI::pause();
    }

    void stats()
    {
        UI::clearScreen();
        UI::title("SYSTEM STATISTICS", C::BMAG);
        int tot = (int)acc.size(), ad = 0, us = 0, ac = 0;
        for (const auto &a : acc)
        {
            if (a.role == Role::ADMIN)
                ++ad;
            else
                ++us;
            if (a.active)
                ++ac;
        }
        UI::section("ACCOUNTS", C::BMAG);
        UI::kv("Total          :", std::to_string(tot));
        UI::kv("Administrators :", std::to_string(ad));
        UI::kv("Regular Users  :", std::to_string(us));
        UI::kv("Active         :", C::BGRN + std::to_string(ac) + C::R);
        UI::kv("Locked         :", C::BRED + std::to_string(tot - ac) + C::R);
        UI::section("SYSTEM", C::BMAG);
        UI::kv("Active Alerts  :", std::to_string(al.alerts().size()));
        UI::kv("Log Entries    :", std::to_string(lg.count()));
        UI::kv("Weather Mode   :", gWx.isMock() ? "DEMO" : "LIVE (OpenWeatherMap)");
        UI::kv("AI Chatbot     :", gBot.isMock() ? "DEMO" : "LIVE (" + gBot.provName() + ")");
        UI::kv("Users File     :", UserStore::path());
        UI::kv("Config File    :", ConfigStore::path());
        UI::kv("Alerts File    :", AlertSys::path());
        std::cout << "\n";
        UI::pause();
    }

    void configKeys()
    {
        UI::clearScreen();
        UI::title("CONFIGURE API KEYS", C::BYLW);
        std::cout << "\n"
                  << C::BYLW << C::B << "  Current Configuration:\n"
                  << C::R;
        UI::kv("Weather API :", UI::badge(gWx.isMock() ? "NOT SET" : "SET", !gWx.isMock()));
        UI::kv("AI Chatbot  :", UI::badge(gBot.isMock() ? "NOT SET" : "SET (" + gBot.provName() + ")", !gBot.isMock()));
        UI::section("SUPPORTED AI PROVIDERS", C::BYLW);
        std::cout << C::WHT
                  << "  Groq       : key starts with gsk_\n"
                  << "  Gemini     : key starts with AIza\n"
                  << "  Anthropic  : key starts with sk-ant-\n"
                  << "  OpenAI     : key starts with sk-\n"
                  << "  Custom URL : any key + your own endpoint\n\n"
                  << C::R;
        UI::menuBox("OPTIONS", {{"1", "Set Weather API Key (OpenWeatherMap)"}, {"2", "Set AI Chatbot API Key (auto-detected)"}, {"3", "Set Custom AI Endpoint URL"}, {"0", "Back"}}, C::BYLW);
        int ch = UI::menu(0, 3);
        if (ch == 1)
        {
            std::string k = UI::input("OpenWeatherMap API Key");
            if (!k.empty())
            {
                gWx.setKey(k);
                ConfigStore::save(k, gBot.key(), gBot.url());
                UI::ok("Weather key saved.");
            }
        }
        else if (ch == 2)
        {
            std::string k = UI::input("AI API Key");
            if (!k.empty())
            {
                gBot.configure(k);
                ConfigStore::save(gWx.key(), k, "");
                UI::ok("AI provider: " + gBot.provName() + "  (saved)");
            }
        }
        else if (ch == 3)
        {
            std::string k = UI::input("API Key");
            std::string url = UI::input("Full API URL");
            if (!k.empty() && !url.empty())
            {
                gBot.configure(k, url);
                ConfigStore::save(gWx.key(), k, url);
                UI::ok("Custom endpoint saved.");
            }
        }
        UI::pause();
    }

    void sysStatus()
    {
        UI::clearScreen();
        UI::title("SYSTEM STATUS", C::BGRN);
        auto row = [](const std::string &n, bool good)
        {
            std::cout << C::WHT << "  " << std::setw(34) << std::left << n
                      << UI::badge(good ? "OPERATIONAL" : "OFFLINE / DEMO", good) << C::R << "\n";
        };
        std::cout << "\n";
        row("Weather API Service :", !gWx.isMock());
        row("AI Chatbot Service :", !gBot.isMock());
        row("Alert System :", true);
        row("Alert File Persistence :", true);
        row("User Authentication :", true);
        row("File User Store :", true);
        row("Config Persistence :", true);
        row("Logging System :", true);
        row("HTTP Client (WinHTTP) :", true);
        if (!gBot.isMock())
            UI::kv("\n  AI Provider :", gBot.provName());
        std::cout << "\n";
        lg.log(u.uname, "STATUS", "");
        UI::pause();
    }
};

// ============================================================================
//  SIGNUP & LOGIN
// ============================================================================
bool doSignup(std::vector<User> &acc, Log &lg, Role r)
{
    UI::clearScreen();
    std::string roleLabel = (r == Role::ADMIN) ? "ADMINISTRATOR" : "USER";
    UI::title("REGISTER -- " + roleLabel, C::BGRN);
    std::cout << "\n"
              << C::BGRN << "  Creating a new " << roleLabel << " account.\n\n"
              << C::R;

    if (r == Role::ADMIN)
    {
        UI::inf("Administrator registration requires an authorization key.");
        if (UI::password("Admin Authorization Key") != ADMIN_KEY)
        {
            UI::err("Invalid key. Access denied.");
            UI::pause();
            return false;
        }
        UI::ok("Authorization accepted.");
    }

    std::cout << "\n";
    std::string uname;
    while (true)
    {
        uname = UI::input("Choose Username");
        if (uname.empty())
        {
            UI::err("Username cannot be empty.");
            continue;
        }
        if (UserStore::exists(acc, uname))
        {
            UI::err("Username '" + uname + "' already taken.");
            continue;
        }
        break;
    }

    std::string pwd;
    while (true)
    {
        pwd = UI::password("Choose Password (min 6 chars)");
        std::string c = UI::password("Confirm Password");
        if ((int)pwd.size() < 6)
        {
            UI::err("Password too short.");
            continue;
        }
        if (pwd != c)
        {
            UI::err("Passwords do not match.");
            continue;
        }
        break;
    }

    std::string fn = UI::input("Full Name");
    std::string em = UI::input("Email Address");
    std::string dp = (r == Role::ADMIN) ? UI::input("Department") : "";
    std::string lc = UI::input("Location (City, Country)");

    acc.emplace_back(uname, pwd, fn, em, r, dp, lc);
    UserStore::save(acc);
    lg.log(uname, "SIGNUP", User::r2s(r));

    std::cout << "\n";
    UI::ok("Account created! You can now log in.");
    UI::pause();
    return true;
}

User *doLogin(std::vector<User> &acc, Log &lg, Role r)
{
    UI::clearScreen();
    std::string roleLabel = (r == Role::ADMIN) ? "ADMINISTRATOR" : "USER";
    UI::title("LOGIN -- " + roleLabel, C::BMAG);
    std::cout << "\n"
              << C::BMAG << "  Enter your " << roleLabel << " credentials.\n\n"
              << C::R;

    for (int attempt = 1; attempt <= 3; ++attempt)
    {
        std::string un = UI::input("Username");
        std::string pw = UI::password("Password");
        for (auto &a : acc)
        {
            if (a.uname == un && a.role == r && a.auth(pw))
            {
                a.recordLogin();
                UserStore::save(acc);
                lg.log(un, "LOGIN", "role=" + roleLabel);
                std::cout << "\n";
                UI::ok("Welcome back, " + a.name + "!");
                UI::pause();
                return &a;
            }
        }
        lg.log(un, "LOGIN_FAIL", "attempt " + std::to_string(attempt), "WARNING");
        UI::err("Invalid credentials. Attempt " + std::to_string(attempt) + "/3.");
    }

    UI::err("Too many failed attempts. Access locked.");
    lg.log("?", "LOGIN_LOCKED", "role=" + roleLabel, "CRITICAL");
    UI::pause();
    return nullptr;
}

// ============================================================================
//  MAIN APPLICATION
// ============================================================================
class App
{
    std::vector<User> acc;
    AlertSys al;
    Log lg;

    void seed()
    {
        if (!acc.empty())
            return;
        acc.emplace_back("admin", "Admin@123", "System Administrator",
                         "admin@stormwatch.com", Role::ADMIN, "IT Operations", "Islamabad, PK");
        acc.emplace_back("user1", "User@123", "Ahmed Khan",
                         "ahmed@stormwatch.com", Role::USER, "", "Karachi, PK");
        UserStore::save(acc);
        UI::inf("Default accounts created in users.txt");
    }

    void splash()
    {
        UI::clearScreen();

        // ---------------------------------------------------------------
        //  StormWatch ASCII Art Banner
        // ---------------------------------------------------------------
        std::cout << C::BCYN << C::B;
        std::cout << "\n";
        std::cout << "   _____ _                   __        __    _       _     \n";
        std::cout << "  / ____| |                  \\ \\      / /_ _| |_ ___| |__  \n";
        std::cout << " | (___ | |_ ___  _ __ _ __ __\\ \\ /\\ / / _` | __/ __| '_ \\ \n";
        std::cout << "  \\___ \\| __/ _ \\| '__| '_ ` _ \\ V  V / (_| | || (__| | | |\n";
        std::cout << "  ____) | || (_) | |  | | | | | \\_/\\_/ \\__,_|\\__\\___|_| |_|\n";
        std::cout << " |_____/ \\__\\___/|_|  |_| |_| |_|                          \n";
        std::cout << C::R;

        std::cout << C::BBLU << C::B;
        std::cout << "  ============================================================\n";
        std::cout << "          W E A T H E R   M A N A G E M E N T   S Y S T E M \n";
        std::cout << "  ============================================================\n";
        std::cout << C::R;

        // ---------------------------------------------------------------
        //  Group Members
        // ---------------------------------------------------------------
        std::cout << C::BCYN << C::B;
        std::cout << "  ============================================================\n";
        std::cout << "   GROUP MEMBERS\n";
        std::cout << "  ============================================================\n";
        std::cout << C::R;
        std::cout << C::BWHT;
        std::cout << "   Muhammad Tahir Hussain        2025-AI-81\n";
        std::cout << "   Muhammad Anas Alam            2025-AI-86\n";
        std::cout << "   Amir Hamza                    2025-AI-97\n";
        std::cout << "   Muhammad Abubakar             2025-AI-98\n";
        std::cout << C::R;
        std::cout << C::BCYN << C::B;
        std::cout << "  ============================================================\n";
        std::cout << C::R << "\n";

        // ---------------------------------------------------------------
        //  Status
        // ---------------------------------------------------------------
        if (gWx.isMock())
            UI::wrn("Weather: DEMO mode  (set key via Admin > Configure API Keys)");
        else
            UI::ok("Weather: LIVE");

        if (gBot.isMock())
            UI::wrn("AI Chatbot: DEMO mode  (set key via Admin > Configure API Keys)");
        else
            UI::ok("AI Chatbot: LIVE -- " + gBot.provName());

        std::cout << "\n  Users file  : " << C::CYN << UserStore::path() << C::R << "\n";
        std::cout << "  Config file : " << C::CYN << ConfigStore::path() << C::R << "\n\n";
        lg.log("SYSTEM", "STARTUP", "StormWatch Weather Management System");
    }

public:
    App()
    {
        acc = UserStore::load();
        seed();
        ConfigStore::load();
        al.load();
        if (al.alerts().empty())
        {
            al.add("Fog Advisory", "MODERATE",
                   "Dense fog expected in northern regions.",
                   "2025-01-15 05:00", "2025-01-15 10:00", "SYSTEM");
            al.add("Heat Wave", "SEVERE",
                   "Temperatures above 42 C in Sindh. Stay hydrated.",
                   "2025-01-15 12:00", "2025-01-17 20:00", "SYSTEM");
        }
    }

    void run()
    {
        splash();
        UI::pause();
        while (true)
        {
            UI::clearScreen();
            UI::title("STORMWATCH WEATHER MANAGEMENT SYSTEM", C::BCYN);
            UI::menuBox("MAIN MENU", {{"1", "Login as User"}, {"2", "Login as Administrator"}, {"3", "Register as User"}, {"4", "Register as Administrator  (requires key: " + ADMIN_KEY + ")"}, {"0", "Exit"}}, C::BCYN);
            std::cout << "\n"
                      << C::DIM
                      << "  Demo credentials:  user1 / User@123    admin / Admin@123\n"
                      << C::R;
            int ch = UI::menu(0, 4);
            if (ch == 0)
                break;

            if (ch == 1 || ch == 2)
            {
                Role r = (ch == 1) ? Role::USER : Role::ADMIN;
                User *user = doLogin(acc, lg, r);
                if (!user)
                    continue;
                if (user->role == Role::ADMIN)
                {
                    AdminDash d(*user, acc, al, lg);
                    d.run();
                }
                else
                {
                    UserDash d(*user, acc, al, lg);
                    d.run();
                }
                lg.log(user->uname, "LOGOUT", "");
                UI::clearScreen();
                std::cout << C::BCYN << C::B << "\n  You have been logged out.\n\n"
                          << C::R;
                UI::pause();
            }
            else
            {
                Role r = (ch == 3) ? Role::USER : Role::ADMIN;
                doSignup(acc, lg, r);
            }
        }

        UI::clearScreen();
        std::cout << C::BCYN << C::B;
        std::cout << "\n  ============================================\n";
        std::cout << "   Thank you for using StormWatch!\n";
        std::cout << "   Stay safe. Stay informed.\n";
        std::cout << "  ============================================\n";
        std::cout << C::R;
        lg.log("SYSTEM", "SHUTDOWN", "clean exit");
    }
};

// ============================================================================
//  ENTRY POINT
// ============================================================================
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h != INVALID_HANDLE_VALUE)
    {
        DWORD m = 0;
        GetConsoleMode(h, &m);
        SetConsoleMode(h, m | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    try
    {
        App a;
        a.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "\nFATAL: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

/*
 ============================================================
  COMPILATION
 ============================================================
  Windows:
    g++ -std=c++17 -o module module.cpp -lwinhttp

 ============================================================
  DEFAULT CREDENTIALS
 ============================================================
  Admin   : admin  / Admin@123
  User    : user1  / User@123

 ============================================================
  ADMIN REGISTRATION KEY
 ============================================================
  ADMIN-2024

 ============================================================
  AI KEY AUTO-DETECTION
 ============================================================
  gsk_...      -> Groq
  AIza...      -> Google Gemini
  sk-ant-...   -> Anthropic Claude
  sk-...       -> OpenAI
  key + URL    -> Custom OpenAI-compatible endpoint
 ============================================================
*/
