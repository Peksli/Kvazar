# Kvazar 🚀
**Game Engine on Vulkan**

Kvazar — это экспериментальный игровой движок, написанный на **C++** с использованием **Vulkan API**.  
Цель проекта — создать современную и гибкую архитектуру для работы с графикой, шейдерами и ресурсами, а также изучить практики низкоуровневого управления GPU.

---

## 🔧 Технологический стек
- **Язык:** C++  
- **Сборка:** CMake  
- **Скрипты:** `.bat` файлы для автоматизации запуска и отладки  
- **Отладка и профилирование:**  
  - [RenderDoc](https://renderdoc.org/) — захват и анализ кадров  
  - [NVIDIA Nsight Graphics](https://developer.nvidia.com/nsight-graphics) — профилирование GPU и анализ ресурсов  

---

## 📦 Сторонние библиотеки
- **[GLFW](https://www.glfw.org/)** — создание окон и обработка ввода  
- **[spdlog](https://github.com/gabime/spdlog)** — высокопроизводительное логирование  
- **[vk-bootstrap](https://github.com/charles-lunarg/vk-bootstrap)** — упрощение инициализации Vulkan (instance, device, swapchain)  
- **[Vulkan Memory Allocator (VMA)](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator)** — управление памятью и аллокациями  

---

## 🎯 Планы развития
- Архитектура для работы с **шейдерами**  
- Система управления **графическими пайплайнами**  
- Поддержка **дескрипторов и пулов**  
- Расширение инструментов для отладки и профилирования  

---

## 🖼️ Отладка и профилирование

### Первый опыт с **NVIDIA Nsight**
![Nsight Screenshot](Results/Screenshots/Nsight_1.jpg)

Описание: подключение к процессу, захват GPU Trace, анализ ресурсов и пайплайнов. Nsight помогает выявлять узкие места и проверять правильность использования памяти и синхронизации.  

---

### Первый опыт с **RenderDoc**
![Nsight Screenshot](Results/Screenshots/Renderdoc_1.jpg)

Описание: захват кадра, просмотр draw‑call’ов, анализ содержимого буферов и текстур. RenderDoc удобен для пошагового изучения рендеринга и проверки корректности данных.  

---

## 🚀 Как собрать проект
```bash
git clone https://github.com/yourname/Kvazar.git
cd Kvazar
cmake -B build
cmake --build build
