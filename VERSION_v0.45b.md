# Shining Saturn 360 — v0.45b

## Marco
Correção de regressões na CPU SH-2 e validação automatizada do barramento/MMIO, com pipeline Xenon e empacotamento XEX concluídos.

## Alterações incluídas
- Corrigida a prioridade de decodificação para `BT`, `BF`, `BT/S` e `BF/S`, evitando que o padrão genérico de MOV capture opcodes de desvio.
- Adicionados casos explícitos de regressão para `BT` e `BF`; os testes de fluxo de controle passaram no CI.
- Ajustado o runner de testes para vincular `src/saturn/sh2_core.cpp` à suíte do barramento.
- Corrigido o endereço usado pelo teste MMIO de 32 bits: o caso agora acessa a região de registradores VDP2 mapeada, em vez de ultrapassar o tamanho da região VDP1.
- Mantido o histórico de diagnóstico das falhas intermediárias.

## Validação de CI
Execução: https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38691857824

Resultado observado:
- `prepare-source`: sucesso.
- `host-regression-tests`: sucesso.
- `build-xenon-elf32`: sucesso.
- `package-xex`: sucesso; artefato `default.xex` e ZIP de versão gerados.

A aprovação é de compilação e testes automatizados. Não comprova inicialização em Xbox 360 físico nem compatibilidade com jogos comerciais.

## Limitações conhecidas
- O interpretador SH-2 continua parcial e ainda não cobre todas as instruções, exceções, privilégios e temporização.
- O MMIO é majoritariamente armazenamento genérico, sem efeitos laterais fiéis dos dispositivos.
- VDP1/VDP2 ainda não constituem um pipeline gráfico completo para jogos.
- SCU/DMA, CD Block, SMPC e SCSP precisam de implementação e integração adicionais.
- Não há compatibilidade de jogos comerciais comprovada nesta versão.

## Próximo marco
v0.46b: expandir cobertura e testes SH-2 com foco em instruções de controle/estado e casos-limite; fortalecer testes do barramento e iniciar handlers MMIO específicos por dispositivo. Não declarar subsistemas completos sem testes próprios.
