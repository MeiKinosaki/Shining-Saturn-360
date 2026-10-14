# Shining Saturn 360 — v0.44b

## Marco
Integração experimental do executor SH-2 com um ciclo principal por quadros.

## Implementado
- `SaturnMainLoop` mantém instâncias da CPU SH-2 e do barramento Saturn.
- Cada chamada a `run_frame(instruction_budget)` executa um orçamento limitado de instruções e avança um contador de quadros.
- Teste de regressão verifica execução contínua de instruções NOP em Work RAM baixa.
- Testes de regressão do SH-2 Runner e da camada genérica MMIO foram aprovados no GitHub Actions.
- O script `tests/run-sh2-core-tests.sh` inclui o teste do ciclo principal.

## Validação
CI do ciclo principal: https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38503506652

CI MMIO: https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38503177536

Essas execuções validam os testes automatizados que elas executam. Não comprovam funcionamento em hardware físico nem compatibilidade com jogos.

## Limitações
- O interpretador SH-2 continua parcial; opcodes não implementados interrompem a execução.
- O ciclo não modela clocks reais, temporização de linha/quadro, arbitragem de barramento, dois SH-2, DMA ou sincronização com periféricos.
- O banco MMIO é genérico e ainda não está conectado a handlers específicos de VDP1, VDP2, SCU, SCSP, CD Block ou SMPC.
- VDP1/VDP2 não produzem ainda uma imagem de jogo; CD-ROM e áudio não estão implementados.
- Não há confirmação de que este núcleo esteja integrado ao `default.xex` nem de boot em Xbox 360 físico.

## Próximo marco
v0.45b: ligar a decodificação do barramento a handlers MMIO específicos, preservando testes para largura de acesso, reset, endereços não mapeados e efeitos laterais dos registradores.
