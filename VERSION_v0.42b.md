# Shining Saturn 360 — v0.42b

## Marco desta versão: mapa físico inicial e regressão

**Versão de desenvolvimento: v0.42b (experimental).**

Esta versão registra a inclusão de constantes de regiões físicas do Saturn e de testes de limites. Ela não representa um emulador completo nem suporte validado a jogos.

### Alterações
- `src/saturn/saturn_memory_map.hpp`: constantes para regiões principais do mapa físico, incluindo Boot ROM, work RAM, SMPC, CD block, SCSP, VDP1, VDP2, SCU e região on-chip do SH-2.
- `tests/test_saturn_memory_map.cpp`: verificações em tempo de compilação e execução de limites de endereço.
- `tests/run-sh2-core-tests.sh`: novo teste adicionado à suíte automatizada.

### Validação
A execução do GitHub Actions associada ao commit do runner é acompanhada em:
https://github.com/MeiKinosaki/Shining-Saturn-360/actions

Não marque esta versão como validada até que os testes de regressão e a compilação Xenon terminem com sucesso.

### Limitações conhecidas
- As constantes são um ponto de partida e ainda precisam de revisão contra documentação técnica confiável, aliases/espelhamentos e diferenças entre SH-2 e dispositivos.
- O mapa ainda não está conectado ao MemoryBus nem ao núcleo SH-2.
- O SH-2 é parcial; VDP1, VDP2, SCSP, CD-ROM, SMPC, temporização e controles não estão integrados como subsistemas funcionais.
- O default.xex é um bootstrap técnico. Inicialização em Xbox 360 real e execução de jogos continuam não confirmadas.

### Próximos marcos
1. Validar a suíte e a compilação desta alteração.
2. Revisar o mapa e implementar um decodificador de endereços que ligue o mapa ao barramento.
3. Implementar testes para leituras/escritas mapeadas e registradores simulados.
4. Integrar o SH-2 ao bootstrap e estabelecer um ciclo de execução visível.
5. Desenvolver dispositivos Saturn progressivamente e testar em hardware quando disponível.
