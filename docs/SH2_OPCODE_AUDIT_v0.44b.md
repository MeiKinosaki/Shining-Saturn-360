# Auditoria SH-2 — linha de base v0.44b

Data: 2026-10-14  
Repositório: https://github.com/MeiKinosaki/Shining-Saturn-360  
Regra de versão: manter v0.44b até que a próxima alteração de código e seus testes sejam validados.

## Escopo observado

Arquivos lidos na branch main:
- `src/saturn/sh2_core.hpp`
- `src/saturn/sh2_core.cpp`
- `tests/test_sh2_core.cpp`
- `tests/run-sh2-core-tests.sh`
- `VERSION_v0.44b.md`

O núcleo possui 16 registradores gerais e os registradores PC, PR, GBR, VBR, MACH, MACL e SR. O interpretador já cobre um subconjunto útil de movimentação, aritmética/lógica, multiplicação, deslocamentos, cargas/armazenamentos, comparações e desvios. O retorno `UnsupportedOpcode` confirma que a implementação ainda não é uma CPU SH-2 completa.

## Lacunas prioritárias para a próxima rodada

A lista abaixo é um plano de auditoria/implementação, não uma afirmação de que cada item foi validado contra o manual.

1. **Auditoria de cobertura de opcodes:** construir uma matriz opcode → handler → teste, comparando os padrões de 16 bits com o manual SH-2. Incluir instruções ainda ausentes como divisão (`DIV0U`, `DIV0S`, `DIV1`), `MUL.W`, MAC, `TAS.B), comparações e operações de registradores especiais.
2. **Registradores especiais e controle:** instruções `STC/LDC`, `STS/LDS`, `RTE`, `TRAPA` e `SLEEP`, com privilégios, exceções e semântica de pilha/vetores corretamente modelados antes de marcar suporte.
3. **Acesso GBR e campos de bits:** revisar cargas/armazenamentos relativos a GBR, `TST`/AND/XOR/OR de memória e variantes imediatas; acrescentar testes de endereço, sinalização e efeitos laterais.
4. **Delay slots:** manter testes para BRA/BSR, JMP/JSR, RTS, BRAF/BSRF e BT/S/BF/S, incluindo alvo, PR e opcode no slot. Definir e testar o comportamento arquitetural quando uma instrução de controle aparece no delay slot.
5. **SR e aritmética:** verificar preservação de todos os bits de SR que não devem mudar, carry/borrow, overflow, sinalização e casos extremos. Atualmente várias instruções atualizam apenas T, e a cobertura de exceções/privilege ainda não existe.
6. **Alinhamento e exceções de barramento:** especificar e testar acessos desalinhados, wrap de endereço, falhas de leitura/escrita e exceções; não deixar o comportamento depender acidentalmente do barramento de teste.
7. **Testes de referência:** gerar casos pequenos com a toolchain Saturn SH-2 e confrontar resultados esperados com os testes do interpretador. A toolchain compila software Saturn; não substitui um emulador nem valida por si só a CPU.

## Critério de aceite

- Cada instrução suportada tem pelo menos um teste de resultado e testes de flags/efeitos laterais quando aplicável.
- Casos extremos incluem valores zero, máximo/mínimo, carry/borrow, overflow e deslocamentos de borda.
- O script `tests/run-sh2-core-tests.sh` termina com sucesso sob `-Wall -Wextra -Werror -pedantic`.
- GitHub Actions confirma os jobs relevantes após o commit.
- Não declarar compatibilidade com jogos comerciais nem boot em Xbox 360 físico sem testes específicos nesses ambientes.

## Referências técnicas

- Hitachi SH-1/SH-2 Programming Manual, 3rd edition (Bitsavers): https://archive.org/details/bitsavers_hitachisupmmingManual3rdEdition_11452188
- SaturnSDK GCC para SH-2: https://github.com/SaturnSDK/Saturn-SDK-GCC-SH2

## Estado desta auditoria

Leitura estática do código concluída; nenhuma execução local de testes ou alteração do interpretador foi realizada por este documento. A versão oficial permanece v0.44b. Próxima ação técnica recomendada: criar testes direcionados para as lacunas da matriz de opcodes antes de implementar os handlers, começando por operações simples e isoladas e só depois avançando para exceções e instruções privilegiadas.
