# STORMWATCH v3.0 — Professional Weather Management System

![StormWatch Banner](https://user-images.githubusercontent.com/0000000/stormwatch_banner.png) <!-- (Update image) -->

> **Author:** Muhammad Tahir Hussain (Group 6)  
> **Technology:** C (Win32 GDI) — NO MFC, NO Unicode, NO external libraries  
> **Version:** 3.0

---

## Overview

**StormWatch v3.0** is a modern, GUI-based weather management and analytics system built purely in C using Win32 API and GDI. It is designed for professional use by meteorological teams, system administrators, and climate researchers. StormWatch features live weather data from online APIs, rich visual dashboards, analytics, and management of weather stations, observations, alerts, users, and settings—packed into a single fast native Windows executable.

---

## Table of Contents

- [Features](#features)
- [System Requirements](#system-requirements)
- [Quick Start & Compilation](#quick-start--compilation)
- [Code Structure](#code-structure)
- [Architecture & Technology](#architecture--technology)
- [User Interface Walkthrough](#user-interface-walkthrough)
- [Data Sources & Live Weather](#data-sources--live-weather)
- [Customization & Extensibility](#customization--extensibility)
- [Troubleshooting](#troubleshooting)
- [License](#license)
- [Credits](#credits)

---

## Features

- **Native Win32 GUI**
  - Fast, responsive interface using GDI (no MFC, no Unicode dependencies)
  - Professional dark-themed visuals, live-resizing, modern dashboard layout
- **Secure Multi-User Authentication**
  - Role-based login (Admin, Meteorologist, Viewer)
  - Interactive, animated login and splash screens
- **Live Weather Integration**
  - Automatic fetch from [wttr.in](https://wttr.in) for any city (JSON REST via WinINet)
  - Safe, threaded HTTP fetch never blocks UI
- **Comprehensive Dashboards**
  - Integrated dashboard: stations, records, analytics, live weather, alerts, users, system settings
  - Animated metric cards, responsive tables, custom data widgets
- **Weather Stations Management**
  - Table of up to 8 demo/prod stations, with full attribute display
  - Status, temp, humidity, wind, active/maintenance/offline status indicators
- **Observational Records**
  - Log/view up to 12 (demo) or unlimited (prod) weather records per station
- **Forecasts**
  - 7-day forecast display, animated multi-metric forecast cards, confidence bars
- **Alert Management**
  - View, issue, and monitor weather alerts (multi-severity, color-coded badges)
  - Quick-access alert notification on top bar
- **Analytical Graphs & Trends**
  - Temperature bar charts and humidity line graphs by station
  - Animated, interactive analytics with real-time redraws
- **Live Weather Search**
  - Enter any city, instantly fetch and display live weather (API)
  - Quick search bar and 1-click city buttons
- **User Management**
  - Add/edit users: admin, meteorologist, viewer roles; activation status badges
- **Settings & System Control**
  - Adjustable general, network, alert, and security options
  - All settings shown as key-value pairs in system control panel
- **Polished UI/UX**
  - Custom fonts, rounded rectangles, GDI-based gradients, smooth animation ticks (timer)
  - Mouseover/sidebar navigation, tooltips, status bar for live status
  - Professional-quality design, modern "SaaS dash" feel

---

## System Requirements

- **Operating System:** Windows 10, 11 (Win32 API)
- **Compiler:** GCC/MinGW (tested), or MSVC with C99/C11 support
- **RAM:** 128MB minimum
- **CPU:** Any x86/x64 processor
- **Internet Connection:** Required for live data
- **Dependencies:**  
  - Win32 API (windows.h, gdi32, comctl32, wininet)
  - No external DLLs, no dependencies beyond what’s built-in to Windows

---

## Quick Start & Compilation

### Clone or Download the Source

```sh
git clone https://github.com/<YourRepo>/stormwatch.git
cd stormwatch/
```

### Compile (MinGW recommended)

```sh
g++ prototype_12.cpp -o stormwatch_gui.exe -lgdi32 -lwininet -lcomctl32 -mwindows
```
- For MSVC, adjust libraries as per your environment.
- The executable will be `stormwatch_gui.exe`.

### Run

```sh
stormwatch_gui.exe
```

The splash screen appears first. Click to proceed to login, then use:
- **Admin:** `admin` / `admin123`
- **Meteorologist:** `meteo` / `meteo123`
- **Viewer:** Any username (≥3 chars) and password (≥4 chars)

---

## Code Structure

**Single-file** for coursework purposes. Modular systems:

- **UI & Layout**: All drawing done via GDI, from basic rectangles to full metric cards and charting (see: `MetricCard`, `TRow`, `FillRR` etc).
- **Global State:** Managed via static variables, simulating data model storage for users, records, stations, settings.
- **HTTP Fetch:** Asynchronous/threaded HTTP using WinINet + event messages (`WM_USER+1`).
- **Page System:** `g_page` controls the current dash section, each has a dedicated `PageX` function.

Key C sections:

- `DrawSplash` — Splash + Login
- `DrawSidebar`, `DrawTopbar`, `DrawStatbar` — Main layout blocks
- `PageDash`, `PageStat`, `PageRec`, ... — Individual page drawing (call-based switch)
- **Event Loop/Message Handling:** Standard Win32 `WndProc`
- **Animation:** Timer-powered via `SetTimer`, smooth progress, pulse, and animations

---

## Architecture & Technology

- **Language:** C (strict C89/C99, but compatible C++)
- **UI:** Win32 API (no Unicode = classic ANSI code paths for all Win32 calls)
- **Graphics:** GDI (custom primitives for every component, animation by buffered re-draw)
- **Network:** WinINet for HTTP GET to public APIs
- **Security:** No MFC/C++/C# or dynamic loading; static role demo accounts
- **Resource-light:** No large DLLs, no Unicode bloat, no .NET

---

## User Interface Walkthrough

- **Login:** Secure, animated login with focus switching, error feedback. Use demo credentials or create a viewer on the fly.
- **Sidebar:** Modern, icon-based navigation with badges and current user info.
- **Top Bar:** Page title, live clock, quick action buttons for alerts, refresh, logout.
- **Status Bar:** Live status messages, copyright.
- **Dashboard:** Four key animated metrics, station status table, 7-day forecast.
- **Stations:** Comprehensive tabular view of all weather stations.
- **Records:** Observation log (date, time, multiple metrics).
- **Forecasts:** Daily forecast, confidence markers, animated temp bars.
- **Alerts:** Severity-based card display of currently issued weather alerts.
- **Analytics:** Dashboard with metric charts—average temperature, humidity, wind, plus multi-station bar and line graphs.
- **User Management:** List/edit all users and roles, activate/deactivate from UI.
- **Settings:** System and network parameters (view only in demo).

---

## Data Sources & Live Weather

- **Live weather pulled from**: [https://wttr.in](https://wttr.in?format=j1)  
  - City names can be entered or clicked using "quick cities" buttons.
  - Data fetched via HTTP+JSON, parsed and mapped to internal struct.
- **Sample Station/Record/Alert/User data:** Provided for demonstration and testing.  
  - Extend or load from file as needed.

---

## Customization & Extensibility

- **To add new stations/alerts/users:**  
  Edit the respective data arrays in the source. For larger deployments, back or load from file/database.
- **Change theme colors or layout:**  
  Edit the `#define C_COLOR ...` section.
- **Additional features:**  
  Add new pages by expanding `PG_COUNT` and the NAV array, then implement a `PageX` drawing function and call in the main page switch.
- **Integrate other APIs:**  
  Replace or complement WinINet fetch logic with any JSON weather API.

---

## Troubleshooting

- **Compilation errors (Wininet or GDI missing):**
  - Ensure you are compiling on Windows with the proper `-lgdi32 -lwininet -lcomctl32 -mwindows` flags.
- **No window appears or runs slowly:**
  - Requires hardware/software GPU for GDI; ensure Windows supports classic GDI.
  - Some VMs or RDP sessions may lag on animation-heavy canvases.
- **Live Weather not loading:**
  - Check internet connection.
  - [wttr.in] API may rate-limit or block if requests are too frequent or from restricted IPs.

---

## License

This project is released under the MIT License. See [LICENSE](LICENSE) for details.

---

## Credits

- Muhammad Tahir Hussain – UI/logic/Win32 implementation, Group 6  
- [wttr.in](https://wttr.in) – API for live weather integration
- Special thanks to open-source Win32, WinINet, and GDI documentation sources

---

> **Note:** This project is provided for educational, research, and demonstration purposes and is not intended as a real-world operational weather SCADA or mission-critical dashboard without further security and robustness reviews.
