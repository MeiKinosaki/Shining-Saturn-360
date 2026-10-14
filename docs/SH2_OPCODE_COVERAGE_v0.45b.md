# Matriz de cobertura SH-2 — preparação da v0.45b

Data: 2026-10-14  
Base examinada: `main`, núcleo `src/saturn/sh2_core.cpp` e testes `tests/test_sh2_core.cpp`.  
Estado: auditoria estática; não equivale a execução de testes nem à validação integral contra o manual.

## Legenda

- **Presente no código**: foi localizado um handler correspondente.
- **Teste localizado**: há pelo menos um caso explícito na suíte atual.
- **Pendente**: precisa de revisão semântica, ampliação de testes ou implementação.

## Cobertura inicial por família

| Família | Estado observado | Próxima verificação |
|---|---|---|
| MOV imediato e entre registradores | Presente no código; testes localizados | Sinalização de imediato e preservação do estado |
| MOV.B/MOV.W/MOV.L por registrador | Presente; testes localizados | Alinhamento, falhas e efeitos colaterais |
| Carga/armazenamento com pré-decremento e pós-incremento | Presente; testes localizados | Casos em que os registradores-base e destino coincidem |
| Endereçamento relativo a PC | Presente; testes localizados | Limites de endereço e alinhamento arquitetural |
| ADD/SUB e variantes com carry/borrow | Parcial; alguns testes localizados | Overflow, carry-in e preservação de SR |
| Comparações EQ/HS/HI/GE/GT/STR | Presente em parte; testes localizados | Auditoria de todos os predicados e casos extremos |
| AND/OR/XOR/NOT/NEG e imediatas | Parcial; testes localizados | Operações sobre memória e variantes GBR |
| Extensão de sinal, troca de bytes/palavras e XTRCT | Presente; testes localizados | Casos-limite e codificações |
| Deslocamentos e rotações | Parcial; testes localizados | Todas as variantes e efeito exato sobre T |
| MUL.L, MULU.W, MULS.W, DMULU.L, DMULS.L | Presente; testes localizados | Valores extremos e auditoria de MACH/MACL |
| BRA/BSR/BT/BF/BT-S/BF-S/BRAF/BSRF/JMP/JSR/RTS | Presente em parte; testes localizados | Todos os alvos, PR, slots e sequências inválidas |
| DIV0U/DIV0S/DIV1 | Não localizado na inspeção inicial | Implementar somente após definir algoritmo e testes de referência |
| MAC.L/MAC.W e TAS.B | Cobertura não demonstrada | Confirmar codificação e semântica no manual; criar testes |
| STC/LDC/STS/LDS | Cobertura não demonstrada | Definir registradores, privilégios e efeitos em SR |
| TRAPA/RTE/SLEEP e exceções | Cobertura incompleta/não demonstrada | Projetar modelo de exceção, vetores e pilha antes dos handlers |
| Acessos GBR e operações de bits em memória | Cobertura não demonstrada por completo | Matriz opcode → endereço → resultado → flags |
| Interrupções e privilégios | Incompletos | Integrar ao modelo de exceções e ao escalonamento |

## Plano de execução

1. Transformar cada linha em casos de teste rastreáveis, usando o manual SH-2 como referência primária.
2. Priorizar instruções não privilegiadas com semântica isolável antes de exceções e instruções privilegiadas.
3. Para cada opcode, registrar: codificação, resultado, PC, SR/T, registradores alterados, acessos ao barramento e comportamento em casos extremos.
4. Executar `tests/run-sh2-core-tests.sh` e consultar o resultado do GitHub Actions após cada alteração de código.
5. Promover a versão somente depois de haver evidência de testes; manter v0.44b como versão oficial até essa validação.

## Limitações

Esta é uma fotografia estática do código consultado, não uma contagem exaustiva de todos os opcodes SH-2. “Não localizado” significa que o handler não foi confirmado na inspeção, não que a instrução foi formalmente provada ausente. Não há nesta etapa afirmação de compatibilidade com jogos comerciais ou de execução no Xbox 360 físico.
