# Shining Saturn 360 — v0.43b

## Marco: adaptador inicial do barramento Saturn

**Versão de desenvolvimento: v0.43b — experimental, ainda não jogável.**

### Entregas
- Adicionado `src/saturn/saturn_memory_bus.hpp/.cpp`, adaptando o barramento genérico ao mapa físico inicial.
- Mapeadas áreas de RAM de trabalho, backup RAM, RAM de som e memórias de vídeo como armazenamento simples.
- Suporte opcional a uma imagem de Boot ROM fornecida pelo chamador, somente leitura e limitada ao tamanho declarado da região.
- Mantido o decodificador físico para inspeção da região de cada endereço.
- Adicionado `tests/test_saturn_memory_bus.cpp` para ROM somente leitura, leitura/escrita em RAM e regiões ainda não mapeadas.
- Integrado o teste ao script `tests/run-sh2-core-tests.sh`.

### CI
A execução do GitHub Actions desta alteração deve ser verificada antes de marcar a versão como validada:
https://github.com/MeiKinosaki/Shining-Saturn-360/actions

### Limitações e cuidados
- O mapa de endereços continua preliminar; aliases, mirrors e particularidades de acesso ainda precisam de revisão.
- O adaptador ainda não conecta a CPU SH-2 nem implementa MMIO, arbitragem, timing, interrupções ou side effects de dispositivos.
- Os espaços de VDP e SCSP são apenas memória armazenada, não implementações de vídeo ou áudio.
- Não há ainda caminho completo de boot do Saturn nem integração deste adaptador no `default.xex`.
- O funcionamento em hardware Xbox 360 real e a execução de jogos não foram confirmados.

### Próxima etapa
Revisar o mapa com documentação técnica, depois implementar registradores MMIO simulados e testes de acesso, antes de ligar a CPU e os dispositivos reais.
