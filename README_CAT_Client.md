# 🛰️ ESP32 VFO CAT Client

Cliente web para control CAT del ESP32 VFO usando la Web Serial API de Chrome.

## 🚀 Características

### 📡 Control Completo del VFO
- **Frecuencia:** Control preciso de 10 kHz a 225 MHz
- **Pasos:** +/- 1 kHz, 10 kHz, 100 kHz, 1 MHz
- **Bandas:** Botones rápidos para 20m, 40m, 80m, etc.
- **Modos:** LSB, USB, CW, FM, AM, FSK, CW-R, FSK-R
- **TX/RX:** Control de transmisión/recepción

### 🔧 Funcionalidades Técnicas
- **Protocolo CAT:** Compatible con Kenwood TS-480
- **Interfaz Web:** HTML5 + JavaScript nativo
- **Web Serial API:** Conexión directa vía puerto serie
- **Log en tiempo real:** Monitoreo de comandos CAT
- **Responsive:** Funciona en desktop y móvil

## 📋 Requisitos

### 🌐 Navegador
- **Chrome 89+** con Web Serial API habilitada
- **Edge 89+** (basado en Chromium)
- **Opera 76+** (basado en Chromium)

### ⚙️ Activar Web Serial API (si es necesario)
1. Ir a: `chrome://flags/#enable-experimental-web-platform-features`
2. Habilitar: **Experimental Web Platform features**
3. Reiniciar Chrome

### 📱 ESP32 VFO
- Firmware con control CAT vía Bluetooth
- Dispositivo Bluetooth: `ESP32_VFO_CAT`

## 🛠️ Uso

### 1️⃣ Preparar el ESP32 VFO
```bash
# Compilar y subir firmware
pio run --target upload
```

### 2️⃣ Emparejar Bluetooth
1. **Windows:** Configuración → Bluetooth → Agregar dispositivo
2. **Buscar:** `ESP32_VFO_CAT`
3. **Emparejar:** Sin PIN requerido
4. **Verificar:** Puerto COM asignado (ej: COM7)

### 3️⃣ Abrir Cliente Web
```bash
# Opción 1: Abrir archivo directamente
# Doble clic en cat_client.html

# Opción 2: Servidor local (recomendado)
python -m http.server 8000
# Ir a: http://localhost:8000/cat_client.html
```

### 4️⃣ Conectar y Controlar
1. **Clic:** "Conectar Puerto Serie"
2. **Seleccionar:** Puerto COM del ESP32
3. **¡Listo!** Control completo del VFO

## 📊 Comandos CAT Soportados

| Comando | Función | Ejemplo |
|---------|---------|---------|
| `FA;` | Obtener frecuencia | `FA00014074000;` |
| `FA<freq>;` | Establecer frecuencia | `FA00014074000;` |
| `MD;` | Obtener modo | `MD3;` |
| `MD<mode>;` | Establecer modo | `MD3;` (CW) |
| `TX;` | Activar transmisión | `TX;` |
| `RX;` | Activar recepción | `RX;` |
| `TQ;` | Estado TX/RX | `TQ0;` (RX) |
| `ID;` | ID del radio | `ID019;` |
| `IF;` | Estado completo | `IF...` |

### 🔢 Códigos de Modo
- `1` = LSB
- `2` = USB  
- `3` = CW
- `4` = FM
- `5` = AM
- `6` = FSK
- `7` = CW-R
- `9` = FSK-R

## 🎛️ Interfaz del Cliente

### 📻 Panel de Frecuencia
- **Display digital:** Frecuencia actual en MHz.kHz.Hz
- **Input manual:** Introducir frecuencia directa
- **Botones de paso:** ±1kHz, ±10kHz, ±100kHz, ±1MHz
- **Bandas preset:** 20m, 40m, 80m, 160m, 15m, 10m, 6m, 2m

### ⚙️ Panel de Control
- **Selector modo:** Dropdown con todos los modos
- **TX/RX:** Botones con indicación visual
- **Estado:** Botón para consultar estado completo
- **ID Radio:** Consultar identificación del equipo

### 📋 Log de Comandos
- **Tiempo real:** Todos los comandos TX/RX
- **Timestamp:** Hora de cada comando
- **Color coding:** Comandos enviados vs recibidos
- **Auto-scroll:** Se desplaza automáticamente

## 🔧 Solución de Problemas

### ❌ "Web Serial API no soportada"
- Usar Chrome 89+ o Edge 89+
- Habilitar flags experimentales
- Verificar HTTPS (necesario para algunos casos)

### ❌ "No se encuentra dispositivo"
- Verificar emparejamiento Bluetooth
- Comprobar que ESP32 esté encendido
- Revisar que no esté conectado desde otra aplicación

### ❌ "Error de conexión"
- Cerrar otras aplicaciones que usen el puerto
- Reiniciar Bluetooth del PC
- Verificar configuración de puerto (115200 baud)

### ❌ "No hay respuesta CAT"
- Verificar firmware ESP32 con CAT habilitado
- Comprobar log de comandos
- Reiniciar ESP32

## 🔍 Debug

### 📱 ESP32 VFO
```bash
# Monitor serial del ESP32
pio device monitor

# Verificar mensajes CAT
[timestamp] CAT TX: FA00014074000;
[timestamp] CAT RX: FA;
```

### 🌐 Navegador
```javascript
// Console del navegador (F12)
// Ver errores de Web Serial API
console.log("Debugging CAT commands");
```

## 📈 Mejoras Futuras

- [ ] **Memoria de frecuencias:** Guardar/cargar frecuencias favoritas
- [ ] **Bandplan:** Mostrar bandplan de radioaficionado
- [ ] **Waterfall:** Display espectral (si se añade al ESP32)
- [ ] **Macros:** Secuencias de comandos programables
- [ ] **Themes:** Modo oscuro/claro

## 🤝 Compatibilidad

### ✅ Compatible con:
- Ham Radio Deluxe
- WSJT-X  
- fldigi
- N1MM Logger
- Cualquier software CAT Kenwood

### 📱 Plataformas:
- ✅ Windows 10/11
- ✅ macOS Big Sur+
- ✅ Linux (Chrome)
- ⚠️ Android (experimental)
- ❌ iOS (no soporta Web Serial API)

¡Disfruta del control completo de tu ESP32 VFO! 🎉