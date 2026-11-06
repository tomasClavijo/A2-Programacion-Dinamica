#!/usr/bin/env bash
# Compila cada ejercicio y lo corre contra sus casos de prueba (Pruebas/*.in.txt vs *.out.txt).
# Uso: ./run_tests.sh            (todos)
#      ./run_tests.sh 03         (solo las carpetas que empiezan con 03)
set -u
cd "$(dirname "$0")"
mkdir -p build
fallas=0

for dir in ${1:-}*/; do
  dir="${dir%/}"
  [ "$dir" = "build" ] && continue
  src=$(ls "$dir"/*.cpp 2>/dev/null | head -1)
  [ -z "$src" ] && continue
  bin="build/$(basename "$dir")"
  echo "== $dir"
  if ! g++ -std=c++17 -O2 -o "$bin" "$src"; then
    echo "   ERROR de compilación"; fallas=$((fallas+1)); continue
  fi
  if [ ! -d "$dir/Pruebas" ]; then
    "./$bin"; continue
  fi
  for in in "$dir"/Pruebas/*.in.txt; do
    out="${in%.in.txt}.out.txt"
    nombre=$(basename "$in" .in.txt)
    if diff -wq <("./$bin" < "$in") "$out" > /dev/null; then
      echo "   OK    $nombre"
    else
      echo "   FALLA $nombre"; fallas=$((fallas+1))
    fi
  done
done

echo
[ "$fallas" -eq 0 ] && echo "Todo OK" || echo "$fallas falla(s)"
exit "$fallas"
