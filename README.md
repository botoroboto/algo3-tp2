# Trabajo práctico 2: Algoritmos sobre grafos

## Instalación

Para instalar y correr el proyecto se necesitan algunas cosas:
- Compilador de C++ (como mínimo C++11)
- Cmake para usar con nuestro CMakeLists.txt

Clonar el repositorio y correr cmake, se puede hacer de la siguiente manera:

```
cmake --build ./build --config Release --target ALL_BUILD -j 8 --
```

Esto generará cuatro archivos ejecutables, de nombre `ejercicioX.exe`, en la carpeta "build".

## Correr el ejecutable

A los ejecutables hay que pasarles la instancia a correr por entrada estandar:


```
corriendo ejercicio i

./build/ejercicioi.exe < ./instancias/i/nombreInstancia.in
```


## Estructura del proyecto

El proyecto se divide en algunas carpetas clave:

- `ejercicios`, dentro tiene 4 carpetas con cada ejercicio particular (allí dentro se pueden encontrar los archivos .cpp del programa en sí, archivos de texto con las mediciones de tiempo, entre otras cosas)
- `instancias`, carpeta con las instancias a utilizar en archivos .in (input) y .out (salida esperada) , divididas por ejercicio. 
