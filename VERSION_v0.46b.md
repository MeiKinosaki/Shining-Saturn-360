# Shining Saturn 360 — v0.46b

## Marco
Expansão incremental do núcleo SH-2 com comparação imediata assinada e transferências byte/palavra/palavra longa relativas ao GBR.

## Alterações
- Implementado `CMP/EQ #imm,R0`, com extensão de sinal do imediato de 8 bits e atualização do bit T.
- Implementadas instruções de leitura e escrita relativas ao GBR: `MOV.B/W/L R0,@(disp,GBR)` e `MOV.B/W/L @(disp,GBR),R0`.
- Adicionados testes de regressão para igualdade com imediato negativo/positivo, largura dos acessos, endianness e extensão de sinal em leituras byte/palavra.
- Mantidos os casos de regressão existentes do núcleo SH-2.

## Validação
- Código e testes enviados ao repositório; a execução do CI para este commit precisa ser consultada antes de declarar os testes aprovados.
- Ainda não é evidência de inicialização em Xbox 360 físico ou de compatibilidade com jogos.

## Limitações
- O interpretador SH-2 continua parcial; exceções, interrupções, privilégios e temporização fiel ainda faltam.
- O MMIO permanece majoritariamente genérico e os handlers dos dispositivos não estão completos.
- VDP1/VDP2, SCU/DMA, CD Block, SCSP e SMPC não formam ainda um sistema integrado capaz de executar jogos.
- O bootstrap `default.xex` não prova que o núcleo SH-2 esteja integrado ao executável.

## Próximo marco — v0.47b
Auditar e corrigir semântica de delay slots e cálculo de PC em desvios; expandir testes de fluxo de controle e começar handlers MMIO específicos com efeitos laterais verificáveis. Não promover sem evidência de CI e revisão do diff.
