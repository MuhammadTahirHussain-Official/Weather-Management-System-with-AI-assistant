# 🌩️ STORMWATCH Weather Management System

**Version**: 1.0

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Screenshots](#screenshots)
- [Group Members](#group-members)
- [Quick Start](#quick-start)
- [Usage Guide](#usage-guide)
  - [Main Menu](#main-menu)
  - [User Portal](#user-portal)
  - [Admin Control Panel](#admin-control-panel)
  - [AI Weather Chatbot](#ai-weather-chatbot)
- [Files & Config Structure](#files--config-structure)
- [Extending StormWatch](#extending-stormwatch)
- [Building & Running](#building--running)
- [Default Credentials](#default-credentials)
- [AI Integration Providers](#ai-integration-providers)
- [License](#license)

---

## Overview

StormWatch is a professional, cross-platform **Weather Management System** written in modern **C++17** for Windows terminals. It offers both real-time and mock demo weather data, stores user authentication in plain-text files, and provides an extensible chatbot interface supporting multiple AI providers (Groq, Gemini, OpenAI, Anthropic, and custom endpoints). Designed for institutional use but simple enough for self-study, it features a colorful CLI, role-based dashboards, and robust file-based persistence.

---

## Features

- 🌐 **Current Weather**: View up-to-date conditions for any city, including temperature, feels-like, humidity, wind, pressure, clouds, sunrise/sunset, and more.
- 🗓️ **7-Day Forecast & 24-Hour Outlook**: Tabular, colored views of extended forecasts and hourly breakdowns.
- 🚨 **Alert Management**: Admin can create, view, and clear weather alerts (with severity, type, time window, and issuer).
- 🧑‍💻 **Role-based Access**: Separate dashboards for Admin and User roles with distinct capabilities.
- 🤖 **AI Chatbot**: Integrated CLI weather Q&A, offering practical advice, planning tips, and lightning-quick responses. Supports major providers & local DEMO/mock.
- 🔐 **Authentication & User Management**: File-based user accounts (plain text), admin registration, login recording, and lockout after failures.
- 📝 **System Log**: Tracks all actions, logins, alerts, configuration changes, and shows recent history.
- 🗃️ **Data Persistence**: All users, alerts, configuration, and logs are human-readable and accessible for backups.
- 🛠️ **Colorful Terminal UI**: ANSI styling for clear, attractive output, progress bars, banners, and menu navigation.
- 🔗 **Configurable API Keys**: Easily add your weather provider or AI keys via the admin dashboard.
- 🏫 **User-friendly**: Suitable as a graduation/final-year project, institutional dashboard, or as a didactic demo for API integration.
- 💡 **Well-Commented Code**: Clean, modular organization with rich function/class docstrings.

---

## Screenshots

> _Add your screenshots here to demonstrate the vivid CLI dashboard, colored log, and chatbot experience._

---

## Group Members

- **Muhammad Tahir Hussain** 
- Muhammad Anas Alam 
- Amir Hamza 
- Muhammad Abubakar 

---

## Quick Start

**Requirements:**
- Windows OS (for console color and WinHTTP)
- MinGW g++ or MSVC (C++17)
- Internet connection (for live weather and AI API features)

**To Build:**
```sh
g++ -std=c++17 -o stormwatch module.cpp -lwinhttp
```

**To Run:**
```sh
stormwatch.exe
```

---

## Usage Guide

### Main Menu

Upon launch, you’ll see the StormWatch banner, group members, and the main menu:

- **Login as User** — access weather, forecasts, alerts, and the chatbot.
- **Login as Administrator** — full control of alerts, keys, logs, and system settings.
- **Register** — create new user or administrator (admin registration requires a secret key).
- **Exit** — save all data & quit.

Default demo accounts are generated on first use.

### User Portal

- **Check Current Weather** — Enter any city for up-to-date conditions.
- **7-Day Forecast / 24-Hour Forecast** — Extended forecasts in table format.
- **Active Alerts** — See all live system weather alerts with severity and details.
- **AI Weather Chatbot** — Ask questions about weather, forecasts, safety, or travel advice; get natural-language responses from LMM-AI.
- **My Profile** — View your info, login stats.

### Admin Control Panel

- **System Log** — View up to 30 most recent actions, warnings, and errors.
- **Weather Operations** — Combined current weather and extended forecast lookup.
- **Alert Management** — Issue or clear weather alerts (persisted in alerts.txt).
- **System Statistics** — Counts of users, roles, system files, and operational status.
- **Configure API Keys** — Set/change your weather API and AI chatbot keys. Enables live mode.
- **System Status** — Check operational status of all subsystems.

### AI Weather Chatbot

- Type questions such as:
  - "Is it likely to rain tomorrow in Lahore?"
  - "What are safety precautions for thunderstorms?"
  - "Tell me the forecast for Dubai next week."
- Provider is auto-detected by the API key or can be set to a custom OpenAI-compatible URL.
- DEMO mode provides static, offline mock responses until an API key is set.

---

## Files & Config Structure

- `users.txt` — user account info (username, hashed password, name, email, role, etc)
- `alerts.txt` — all currently active alerts, with type, severity, time window, issuer, etc.
- `config.txt` — API keys for weather data and AI chatbot. Set via admin control panel.
- (On first run, these files are generated if missing.)

---

## Extending StormWatch

- **Weather Provider**:
    - By default, uses mock demo weather unless you set a valid [OpenWeatherMap](https://openweathermap.org/api) API key.
    - See `Configure API Keys` in the Admin panel.

- **AI Chatbot Provider**:
    - Drop in API keys for: Groq (`gsk_...`), Google Gemini (`AIza...`), Anthropic Claude (`sk-ant...`), OpenAI (`sk-...`), or your own custom endpoint.
    - Switching providers is instant via the admin config UI.
    - For custom OpenAI-compatible APIs, supply your endpoint URL in addition to the key.
    - Ask about weather, safety, or forecasts; bot replies in natural language.

- **Data storage**:
    - All user/activity/alert/config files are plain text, comma or bar-delimited — easily extensible and human-readable for backups or imports.

- **Code customization**:
    - Modular architecture: `User`, `AlertSys`, `ConfigStore`, `Log`, `Chatbot`, `WeatherSvc`, and UI helpers are separable or extendable.
    - Add additional role types or dashboard modes by modifying the respective class implementations.

---

## Building & Running

### Windows

```sh
g++ -std=c++17 -o stormwatch module.cpp -lwinhttp
```

- Run `stormwatch.exe` from cmd or PowerShell.

### Notes

- **Color support**: Works best on Windows 10+ terminal with ANSI/VT support.
- **APIs**: By default, runs in DEMO mode (no API keys needed for CLI demo/testing). For real data, you'll need keys.
- **WinHTTP**: Used for all external HTTP requests, including weather & AI.

---

## Default Credentials

First run creates:

| Role     | Username | Password    |
|----------|----------|-------------|
| Admin    | admin    | Admin@123   |
| User     | user1    | User@123    |

### Admin registration key

`ADMIN-2024`

---

## AI Integration Providers

Just paste in your API key via **Admin > Configure API Keys**:

| Provider   | Accepted Key Format   | Notes                        |
|------------|----------------------|------------------------------|
| Groq       | `gsk_...`            | Llama-3, Mixtral, Gemma      |
| Gemini     | `AIza...`            | Google Gemini                |
| Anthropic  | `sk-ant-...`         | Claude                       |
| OpenAI     | `sk-...`              | GPT-3.5, GPT-4, etc.         |
| Custom     | Any key + custom URL  | OpenAI-compatible endpoints  |

---

## License

This system is an original group project created **for educational use** at CUI Wah, © 2025.  
Open-source for academic and demo use only. See code comments for further details.

---

> For questions or contributions, open an issue or contact [Muhammad Tahir Hussain](mailto:mthussain002@gmail.com).
