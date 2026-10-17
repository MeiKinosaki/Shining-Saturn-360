# Continuação de desenvolvimento — Shining Saturn 360 v0.45b

Data: 2026-10-16  
Versão oficial: **v0.45b**. Não promover a v0.46b até validar regressões, build Xenon ELF32 e empacotamento XEX.

## Evidências confirmadas

- CI do commit `43b06a04`: sucesso nos testes host, build Xenon ELF32 e empacotamento `default.xex`/ZIP.
- Testes de `DIV0S` com todas as combinações de sinais corrigidos e aprovados no commit `7338aa6a`.
- Testes de preservação de SR em `CLRT`/`SETT` aprovados no commit `5ffbecd4`.
- Novos testes de borda para `ADDC`/`SUBC` no commit `ccf6c76c`; CI ainda em execução quando esta nota foi escrita.
- Novos testes de `CLRMAC`, `DMULU.L` e `DMULS.L` no commit `c2628c40`; CI ainda em fila quando esta nota foi escrita.

## Estado por área (estimativa, não uma medição automática)

| Área | Estado atual | Próxima ação técnica |
|---|---|---|
| CPU SH-2 | Interprete parcial; testes aritméticos aumentados | Confirmar CI dos novos testes; revisar opcode/flags; depois planejar DIV1 com testes de referência |
| Fluxo de controle/delay slots | Implementação parcial com testes existentes | Cobrir combinações de branch, delay slot e PC |
| Barramento/memória/MMIO | Mapa e acessos de larguras variadas com testes de limites | Aumentar testes integrados de CPU↔bus e de alinhamento |
| SCU/DMA | Protótipo com validação de endereços e round-robin | Integrar interrupções, sincronização e temporização reais |
| VDP1 | Rasterização/comandos parciais | Expandir comandos, clipping e testes de coordenadas |
| VDP2 | Camadas, scroll e prioridades parciais | Testes de composição de camadas e paletas |
| CD Block/disco | Estrutura e leitura de disco incompletas | Definir comandos/sectores e validar imagens suportadas |
| SCSP/áudio | Estrutura inicial | Implementar canais, amostras e temporização |
| SMPC/controles | Estrutura inicial | Comandos do sistema e leitura de controle |
| Inicialização/loader | Integração incompleta | Inicialização ordenada e fluxo de boot reproduzível |
| UI/biblioteca/capas/favoritos | Parcial/planejado | Implementar componentes incrementais sem declarar concluído antecipadamente |
| Save/load states | Inicial | Definir formato versionado e teste de ida-e-volta |
| Toolchain | CI compilou ELF32 e empacotou XEX no commit validado | Revalidar nos commits mais recentes e verificar conteúdo dos artefatos |
| Hardware Xbox 360 | Sem boot físico comprovado | Executar teste real em console RGH/JTAG e registrar resultado |
| Compatibilidade comercial | Nenhum jogo comercial comprovado nesta evidência | Criar matriz de boot/teste por imagem legalmente obtida |

## Regra de promoção de versão

A versão permanece v0.45b até que:
1. os novos testes host passem;
2. o build Xenon ELF32 passe;
3. o empacotamento de `default.xex` passe;
4. os artefatos sejam inspecionados;
5. as limitações de teste em hardware sejam explicitamente registradas.

CI verde confirma apenas os jobs executados; não prova, por si só, boot em console ou compatibilidade com jogos comerciais.

## Atualização incremental — 2026-10-17

- Commit `b0ad2d3c`: ampliou `tests/test_sh2_arithmetic_edges.cpp` com casos de comparação assinada/não assinada, `CMP/STR` e preservação de SR em `AND`, `XOR` e `OR`. CI iniciada; resultado ainda não confirmado.
- Commit `b6b8410d`: acrescentou casos de borda para `SHLL`, `SHLR`, `ROTCL` e `ROTCR`, verificando o bit T e a preservação dos bits não relacionados de SR. CI iniciada; resultado ainda não confirmado.
- Estes commits ampliam a cobertura de testes; não representam por si só novas instruções implementadas nem avanço equivalente em todos os subsistemas.
- A versão oficial continua **v0.45b**. Próxima promoção depende de CI verde para os commits relevantes, inspeção dos artefatos ELF32/XEX e validações documentadas.

## Atualização adicional — 2026-10-17 (rodada de regressões)

- CI do commit `b0ad2d3c`: sucesso; testes de comparações e operações lógicas passaram.
- CI do commit `b6b8410d`: sucesso; testes de deslocamentos/rotações passaram.
- CI do commit `58c4e7ce`: sucesso; a codificação do opcode BRAF no teste foi corrigida após a primeira execução de controle de fluxo falhar.
- CI do commit `8e0cae36`: sucesso; acessos big-endian de 16/32 bits na RAM passaram.
- Commit `946adb05`: adiciona testes para orçamento DMA zero e cancelamento/reset; CI ainda precisa ser confirmada.
- Commit `c96ee324`: adiciona testes para escritas em endereço não mapeado e isolamento entre bytes adjacentes da RAM; CI ainda precisa ser confirmada.
- A execução de controle de fluxo em `841c0a6c` falhou em um teste BRAF; a causa no teste foi corrigida no commit subsequente `58c4e7ce`, cuja CI terminou com sucesso.
- Esses avanços são testes de regressão. Não significam que todas as instruções, dispositivos ou jogos estejam completos. A versão oficial permanece **v0.45b** até validar os commits recentes, inspecionar artefatos e registrar limites de hardware.
