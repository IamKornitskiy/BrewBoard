# BrewBoard

**Interactive beer menu / Интерактивное барное меню**

---

## ⚠️ Project status / Статус проекта

**EN:** The project is **frozen** because Untappd API has suspended new application registrations.  
Without `client_id` and `client_secret` real network requests are impossible.

**RU:** Проект **заморожен** из-за того, что Untappd API прекратил регистрацию новых приложений.  
Без `client_id` и `client_secret` невозможно выполнять реальные сетевые запросы.

---

## Dependencies & Build / Зависимости и сборка

### Dependencies / Зависимости

- Qt 6.5+ (Core, Quick, Qml, Network)
- CMake 3.16+
- C++17 compiler
- (optional) NixOS with `shell.nix`

### Build instructions / Инструкция по сборке

```bash
git clone git@github.com:IamKornitskiy/BrewBoard.git
cd BrewBoard

# If using NixOS
nix-shell

# Configure and build
mkdir build && cd build
cmake ..
make

# Run
./BrewBoard
```

> Note / Примечание: The app will build and launch, but search will fail without valid Untappd API keys.
> Приложение соберётся и запустится, но поиск не будет работать без действующих ключей Untappd API.

## Future plans / Планы на будущее
**EN:**

Create an interactive bar board showing beers on taps (menu).

Use a different API (e.g., Punk API, local JSON) or wait for Untappd policy change.

Add tap status, ABV, IBU, brewery info, and real‑time updates.

**RU:**

Разработать интерактивную барную доску с информацией о пиве на кранах (меню).

Использовать другой API (например, Punk API, локальный JSON) или ждать изменения политики Untappd.

Добавить статус кранов, ABV, IBU, информацию о пивоварне и обновление в реальном времени.

### 📫 Contact
Email: iam@kornitskiy.ru

*Last updated / Последнее обновление: 2026-05-14*
