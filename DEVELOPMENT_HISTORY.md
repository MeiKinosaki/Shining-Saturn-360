# Histórico de desenvolvimento — Shining Saturn 360

Repositório: https://github.com/MeiKinosaki/Shining-Saturn-360  
Plataforma-alvo: Sega Saturn em Xbox 360 RGH/JTAG  
Executável-alvo: `default.xex`  
Política de versões: sequência numérica v0.Nb; não declarar marcos como concluídos antes de testes.

## Propósito
Este documento preserva um resumo cumulativo do histórico técnico e das decisões do projeto. O histórico Git é a fonte autoritativa para cada alteração de código; este arquivo não substitui commits, logs de CI, artefatos ou conversas externas ao repositório.

## Marcos registrados

### v0.25b–v0.29b (histórico recuperado de notas anteriores)
- Expansão e testes do núcleo SH-2 para instruções como SUB, DT, BT/BF, BRA e BSR.
- Os testes locais descritos nas notas passaram.
- Naquele momento, a publicação GitHub encontrou HTTP 403; não inferir que artefatos ZIP tenham sido publicados.
- Estado descrito: núcleo SH-2 parcial, não um emulador Saturn completo.

### v0.41b
- Núcleo interpretador SH-2 experimental e testes de regressão iniciais.

### Testes do núcleo SH-2 e memória
- Commit `99588c1031198b6ade8a83008c79efdd8597ca4c`: teste de reset do SH-2 limpando estado arquitetural; CI informado como aprovado, run 38369020456.
- Commit `fb45dc0c06e4e4755a2988af69ae5c3cf1aa849b`: teste de JSR/RTS e delay slots; CI informado como aprovado, run 38369688154.
- Commit `7ef70f91562d68967095294918f8aa7695c80546`: teste de JMP @Rn com delay slot.
- Mapa de memória inicial: `src/saturn/saturn_memory_map.hpp`; teste `tests/test_saturn_memory_map.cpp`; commits registrados: `ddff286dca6a50434b71c828799bf467951b2c72`, `1ff89c653663360a8fe2ee87681a27ad1838994e`, `e81f318396426d122420c43c3aa7c22c96b4f3ce`.
- Decodificador inicial de endereços: `src/saturn/saturn_address_decoder.hpp`; teste `tests/test_saturn_address_decoder.cpp`; commits: `d7bf249938e314a428a88d6706470882d4d84f1f`, `4a6faaa77605adcb09355e203999c240218a77d8`, `5d36661cb10886274914235efca0c9f36a589074`.

### v0.42b
- Versão e README atualizados para registrar o mapa/decodificador de endereços e os testes associados.

### v0.43b
- Barramento de memória Saturn inicial: `src/saturn/saturn_memory_bus.hpp/.cpp`.
- Teste: `tests/test_saturn_memory_bus.cpp`.
- Commits: `46e22dc4fbab7f5ed84450df5dde709cbc3333f6`, `bf2fb776eee4733d641606d8c36d41c4e46b2dd`.
- Integração SH-2/barramento: `tests/test_sh2_saturn_bus_integration.cpp`; commits `74812f8b281e9c954da8b272e36aa8f1ae67ff2c`, `193c3dab7f29cd02b8774ae70649d4ad4a77ed40`.
- Runner SH-2 com orçamento limitado: `src/saturn/sh2_runner.hpp/.cpp`; teste `tests/test_sh2_runner.cpp`; commits `413a2b6df47f5d331073ab79acb418e937b5ade8`, `38fb5ebe9240bea2639b2169d6df5a7387e33477`, `aedcdb92bae197a83868eeed8d00743179783629`, `d37fd9dd99303487055d80e996f1f34c062ae661`.
- O runner executa até um orçamento de instruções e para quando uma instrução não é suportada.

### v0.44b
- Ciclo principal experimental: `src/saturn/saturn_main_loop.hpp/.cpp`; teste `tests/test_saturn_main_loop.cpp`.
- Registra execução limitada da CPU por chamada e contador de quadros. Não simula temporização real, dual-SH-2, arbitragem de barramento nem sincronização de dispositivos.
- Documento `VERSION_v0.44b.md` e README atualizados.
- Banco genérico MMIO: `src/saturn/saturn_mmio.hpp/.cpp`; teste `tests/test_saturn_mmio.cpp`.
- Integração inicial MMIO/barramento: registradores de armazenamento genérico para faixas associadas a SMPC, SCSP, VDP1, VDP2, SCU e CD Block; ainda sem efeitos laterais reais.
- Correção do script de regressão para compilar `saturn_mmio.cpp`: commit `3ab565e8f9c3781ca47372f77f5ec6cfa91275fb`; CI associado informado como aprovado em run 38504930287.

### Desenvolvimento gráfico experimental (após v0.44b; não declarar versão oficial sem validar CI)
- Framebuffer de software RGB555, 320×224: `src/saturn/vdp1_renderer.hpp/.cpp`; teste `tests/test_vdp1_renderer.cpp`.
- Primitivas iniciais: limpar e preencher retângulos com recorte nas bordas.
- Lista de comandos experimental: `src/saturn/vdp1_command_list.hpp/.cpp`; teste `tests/test_vdp1_command_list.cpp`.
- Comandos próprios de teste: Clear, FillRect e End, com orçamento de execução e rejeição de opcode inválido. Não é o formato real do hardware VDP1.
- Foi corrigida a expectativa de pixel do teste com commit `0b2f92753d0e74bfee024920deb28e85da304faa`; a nota mais recente dizia que o novo CI estava pendente. Consultar GitHub Actions antes de afirmar aprovação.

## Decisões técnicas persistentes
- Projeto: Shining Saturn 360.
- Alvo: Xbox 360 RGH/JTAG; objetivo de executável `default.xex`.
- Manter versões numéricas sequenciais (`v0.45b`, `v0.46b`, etc.) e não pular versões.
- Separar código escrito, CI aprovado, artefato produzido e teste em console físico.
- Engenharia reversa baseada em documentação técnica pública, testes controlados e compatibilidade observável; registrar hipóteses e limitações.

## Estado técnico conhecido (não presumir mudança sem evidência)
- SH-2: interpretador parcial; instruções não implementadas podem interromper execução.
- Main loop: experimental, sem temporização precisa ou sincronização de todos os dispositivos.
- Mapa/barramento: rascunho funcional inicial; aliases, mirrors, latências e semântica integral do hardware não concluídos.
- MMIO: banco genérico, ainda sem handlers completos/efeitos reais dos dispositivos.
- VDP1: framebuffer e comandos artificiais de teste; não interpreta o formato real de comandos Saturn.
- VDP2: composição real de planos/camadas ainda pendente.
- SCU/DMA/interrupções: incompletos.
- CD Block/CD-ROM, SCSP áudio, SMPC/controladores: ainda não são dispositivos funcionais completos.
- Integração completa do núcleo atualizado no `default.xex`: não confirmada.
- Boot e execução em Xbox 360 físico: não confirmados.
- Compatibilidade com jogos comerciais: não confirmada.

## Próximos marcos propostos (metas, não funcionalidades concluídas)
- v0.45b: validar MMIO/barramento e adicionar handlers/semântica por dispositivo.
- v0.46b: ampliar renderização VDP1, com primitivas e testes.
- v0.47b: iniciar composição VDP2 e saída de imagem.
- v0.48b: interrupções e integração inicial SCU.
- v0.49b: estrutura do CD Block e leitura de setores.
- v0.50b: base de SCSP e SMPC.
- Depois: sincronização, toolchain Xenon, geração/validação de `default.xex`, artefatos reprodutíveis e teste no console físico.

## Como atualizar
A cada marco, acrescentar data, versão, resumo, arquivos alterados, hashes dos commits, links do CI e conclusão (success/failure/pending). Não marcar CI como aprovado sem consultar o resultado. Não apagar entradas anteriores; corrigir erros por nota datada.


## Atualização de desenvolvimento — 2026-10-14 (primitivas VDP1)
- Adicionadas APIs `draw_line` e `fill_triangle` ao framebuffer experimental.
- Implementação usa recorte de linhas e rasterização de triângulos preenchidos; RGB555 é limitado a 15 bits.
- Testes adicionados para pixels de uma linha diagonal e preenchimento de triângulo.
- Commits:
  - Header: `a93d027b9c95d79dc41dae7fd060379a72b416e6`
  - Implementação: `2f5ca8d8484fdbd667a57ba972a0c10584196b45`
  - Testes: `74601f9e3af16f72903d70e9f589c0d3b61795f1`
- CI desta alteração: pendente no momento do registro, run https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38539234454.
- Limitação: são primitivas de software para o protótipo, não a interpretação do protocolo real do VDP1; não implica renderização de jogos nem teste em console físico.


## Correção de CI — 2026-10-14
- O CI do marco de primitivas VDP1 falhou na compilação porque as sequências `\\n` foram gravadas literalmente em `vdp1_renderer.hpp` e `tests/test_vdp1_renderer.cpp`, em vez de quebras de linha.
- Corrigidos os dois arquivos para converter as sequências literais em linhas reais.
- Commits corretivos: header `ca6746c71a86ff1f365c6f43cfc0f74b1e3c2997`; teste `8964fd4b815c5ceea753a948ce0bd5ddfad32c8f`.
- CI original que identificou a falha: https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38539234454 (host regression tests falhou na compilação; os jobs de ELF32 e empacotamento XEX passaram).
- CI da correção ainda precisa ser confirmado; não marcar primitivas como validadas até o novo resultado.


## Correção adicional de compilação VDP1 — 2026-10-14
- O CI após a primeira correção mostrou uma segunda ocorrência de `\\n` literal em `src/saturn/vdp1_renderer.cpp`, na diretiva de inclusão.
- Corrigido para diretivas `#include` em linhas separadas.
- Commit: `3f7cd46c254e17c8845c80339825014f66a5e501`.
- CI que identificou a segunda ocorrência: https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38539823772.
- A nova validação após esta correção ainda está pendente.


## Atualização de desenvolvimento — 2026-10-14 (compositor VDP2 experimental)
- CI verde após corrigir quebras de linha no renderer VDP1: run https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38540416556.
- Criado protótipo de compositor de software com duas camadas RGB555, cor de fundo, prioridade inteira, ativação/desativação e composição 320×224.
- Arquivos: `src/saturn/vdp2_compositor.hpp/.cpp`, `tests/test_vdp2_compositor.cpp`; teste registrado em `tests/run-sh2-core-tests.sh`.
- Commits: header `7c88c70b384dd15a57f4c7105d97db2bd88e3589`, implementação `f517634183b68b4d39c89f114fcf1b8fe48ce4dc`, testes `f24cffade3702e6cf6cde77875c29de5a1c3e176`, runner `c7dffb3b789c67fef891a5d0a7b4cdedd9d5c5b7`.
- CI do novo compositor pendente; não marcar como aprovado antes do resultado.
- Limitação: compositor simplificado com buffers fornecidos pelo software; não implementa registradores, planos, tilemaps, scroll, rotação, sprites, color math nem temporização reais do VDP2. Sem saída de vídeo integrada ao console.


## Correção VDP2 — 2026-10-14
- O CI do commit `c7dffb3b789c67fef891a5d0a7b4cdedd9d5c5b7` falhou no teste `test_vdp2_compositor`: a camada com prioridade maior era desenhada primeiro e depois coberta pela camada de prioridade menor.
- Corrigida a ordem de composição em `src/saturn/vdp2_compositor.cpp` para desenhar primeiro a prioridade inferior e deixar a prioridade superior por cima.
- Commit corretivo: `1618b09d83dfae0057f1b3eda99463582e50267c`.
- A compilação Xenon ELF32 e o empacotamento `default.xex` do run anterior passaram, mas os testes de regressão host falharam no compositor. A correção precisa de novo CI antes de declarar o compositor validado.
- Run que revelou a falha: https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38541134347.


## Atualização de desenvolvimento — 2026-10-14 (testes de borda da CPU SH-2)
- Adicionado `tests/test_sh2_arithmetic_edges.cpp` para verificar casos de overflow assinado em ADDV/SUBV, preservação de bits não-T de SR, DT e comparações CMP/PZ e CMP/PL.
- Registrado o novo executável de teste em `tests/run-sh2-core-tests.sh`.
- Commits: teste `c9deb99c5bcb87ea955c35b5c68fb282a2b25f31`; runner `e6f1e8aa2f90a81d82bf77ea7b7e501e06be71e0`.
- CI dos commits de teste está pendente no momento desta atualização: runs https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38600318423 e https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38600340674.
- A execução bem-sucedida do CI anterior não valida esses testes novos; aguardar os runs atuais.
- Limitação: foram adicionados testes, não novas instruções ao interpretador. A versão oficial continua v0.44b; v0.45b segue como marco de desenvolvimento.


## Continuação — testes de delay slots SH-2 (2026-10-14)
- A execução do workflow `38600340674` concluiu com sucesso: regressões host, compilação Xenon ELF32 e empacotamento `default.xex`. Isso valida o pipeline e os testes existentes naquele commit, não execução em console real nem compatibilidade de jogos.
- Adicionado `tests/test_sh2_delay_slots.cpp` com casos de BRA e BSR, incluindo execução da instrução no delay slot e verificação de PR/alvo.
- Commit do novo teste: `716a75af2126fbecfd73cbf8c3a35db49ccdf64e`.
- O runner `tests/run-sh2-core-tests.sh` agora compila/executa esse teste: `ed4a40f351f689b6d5916273efd5619a44896bcf`. A CI deste commit ainda deve ser consultada antes de declarar os novos casos aprovados.
- A versão oficial continua v0.44b; v0.45b permanece em desenvolvimento. Nenhuma instrução nova foi adicionada ao interpretador nesta etapa.


## Continuação — regressões de fluxo de controle SH-2 (2026-10-14)
- O workflow `38600644504` concluiu com sucesso: regressões host, compilação Xenon ELF32 e empacotamento `default.xex`. Isso valida o pipeline e os testes executados naquele commit, não o funcionamento em console físico.
- Adicionado `tests/test_sh2_control_flow.cpp` cobrindo BT/S tomado e não tomado, JMP @Rn e JSR @Rn, incluindo delay slots e PR.
- Commits:
  - Testes de fluxo de controle: `171ccd64e231d085648dfa96be2702f64cb95ccc`
  - Inclusão no runner: `c16d248dcc842f4693bd7cdb8b7f252ec2bfa671`
- A nova CI precisa ser consultada antes de declarar os testes aprovados.
- A versão oficial continua v0.44b; v0.45b segue em desenvolvimento. Não houve alteração do interpretador nesta etapa.


## Continuação — testes de largura de acesso MMIO (2026-10-14)
- Reforcei `tests/test_saturn_memory_bus.cpp` com verificações de acessos de 16 e 32 bits ao intervalo MMIO, confirmando ordem de bytes big-endian e leitura de volta.
- Commit: `99d74d03a45eaf985e3dc9b486715809ce1fb680`.
- O teste já faz parte do runner `tests/run-sh2-core-tests.sh`; a validação do novo commit depende de CI posterior.
- Escopo: o banco MMIO continua sendo armazenamento genérico. Estes testes verificam a camada de acesso, não os efeitos reais dos registradores Saturn.


## Continuação — correção de endereço PC-relative do SH-2 (2026-10-14)
- Auditoria do interpretador encontrou o cálculo-base de `MOV.L @(disp,PC),Rn` alinhado a partir do PC pós-fetch, mas sem o avanço de 2 bytes exigido pela fórmula PC+4 alinhada.
- Corrigido o endereço efetivo em `src/saturn/sh2_core.cpp`, preservando o deslocamento multiplicado por quatro.
- Adicionado caso de regressão no limite de alinhamento em `tests/test_sh2_core.cpp`.
- Commits: correção `576e9290959c6d1e604d9cb56b1e8800bb268064`; teste `dbcb6ebe44065c8db07bc4bac6b03acf34cda456`.
- O novo código e teste ainda precisam de CI posterior; não declarar aprovação antes do resultado.


## Correção de decodificação SH-2 — 2026-10-15
- A regressão `test_sh2_control_flow` falhava porque o padrão amplo de `MOV.B/W R0,@(disp,Rn)` para opcode `0x8xxx` interceptava `BT/S` (`0x8dxx`) antes da decodificação de desvio.
- Movidas as quatro condições `BT`, `BF`, `BT/S` e `BF/S` para antes do padrão genérico de MOV, preservando o restante da execução do core.
- Commit anterior anotado incorretamente; a correção efetiva foi registrada em `593c4fa77d92bd76f871ad2310c78a5bb403301a`.
- A correção está registrada, mas ainda não há resultado de CI associado confirmado; não declarar os testes aprovados até a nova execução.
- Versão oficial permanece v0.44b; v0.45b continua em desenvolvimento.


## Continuação SH-2 e correção do runner — 2026-10-15
- Os testes de fluxo de controle passaram no job de host, incluindo a sequência BT/S com delay slot; o runner então avançou até a suíte do barramento Saturn.
- A execução falhou na ligação de `test_saturn_memory_bus.cpp`: o comando compilava o teste sem `src/saturn/sh2_core.cpp`, que fornece as implementações de `Bus::read16/read32/write16/write32`.
- Corrigido `tests/run-sh2-core-tests.sh` para linkar `sh2_core.cpp` no teste do barramento Saturn.
- Commit da correção do runner: `0f79d590c761a015a7ba0025cca4c8470eecd086`.
- A compilação Xenon ELF32 e o empacotamento XEX dos runs recentes terminaram com sucesso; a suíte completa de regressão ainda precisa ser reexecutada após a correção do runner.
- A cobertura BT/BF foi ampliada com casos explícitos em `tests/test_sh2_control_flow.cpp`, commit `7e5edbbdc0e855eb6ddaeee78c6434940668ad8a`.
- Versão oficial permanece v0.44b; v0.45b não está promovida.


## Regressão MMIO: limite do banco VDP1 — 2026-10-15
- A execução do CI confirmou que os testes SH-2, aritmética, delay slots, fluxo de controle, MemoryBus, GameLibrary, música do menu, scanner de ROM, configurações e mapa físico passaram.
- A suíte parou em `test_saturn_memory_bus`: o caso de escrita de 32 bits usava o endereço `0x05D00020`, fora da região mapeada de registradores VDP1 (`0x18` bytes). O teste estava usando um endereço inválido para o tamanho do dispositivo.
- Corrigido o caso para usar `0x05F80020`, dentro da região de registradores VDP2, mantendo a verificação big-endian de 32 bits.
- Commit: `2005a0cf9a5e36cad178ee8ae4546236875bd4bb`.
- No run `38691675136`, compilação Xenon ELF32 e empacotamento `default.xex` concluíram com sucesso, mas a suíte de regressão falhou no caso MMIO acima. A nova correção ainda aguarda CI.
- Versão oficial permanece v0.44b; v0.45b não está promovida.


## Promoção para v0.45b — 2026-10-15
- O GitHub Actions run `38691857824` terminou com sucesso em todos os jobs relevantes: `host-regression-tests`, `build-xenon-elf32` e `package-xex`.
- A suíte de regressão passou depois da correção do teste MMIO, e o pipeline gerou `default.xex` e o ZIP de versão.
- Promovida a versão do repositório de v0.44b para v0.45b; atualizados `VERSION`, `README.md` e criado `VERSION_v0.45b.md`.
- A aprovação confirma apenas CI/compilação/empacotamento. Boot em hardware físico e compatibilidade com jogos continuam não comprovados.
- Próximo marco: v0.46b, com expansão da cobertura SH-2 e começo de handlers MMIO específicos por dispositivo.


## Trabalho em andamento para v0.46b — 2026-10-15
- Aumentado o compositor VDP2 experimental de 2 para 5 planos de fundo de framebuffer, com ordenação estável por prioridade e teste de sobreposição para cinco planos.
- Criada a suíte `tests/test_sh2_memory_ops.cpp` cobrindo extensão de sinal em cargas byte/word, cargas com pós-incremento, stores com pré-decremento e escrita big-endian de byte/word/longword.
- Incluída a suíte de operações de memória no runner `tests/run-sh2-core-tests.sh`.
- Commits: `5988b35eda5599cda4ce4bee3a769134d0939679`, `48aaf6e90f3c02e2bded0db1adc4423dfd4ef46e`, `dc361b3bd40a52530eb29b102b27febd56fc68c8`, `149ae85be56133db4ac035de2d712e303f41efd0`, `1440fb7d6e0c6b67ed54488e8b872dc938cd75d4`.
- CI run `38718489736` está executando compilação Xenon ELF32 e testes host. Não promover v0.46b antes de todos os jobs relevantes terminarem com sucesso.
- VDP2 continua sendo compositor de framebuffer experimental, não um renderer Saturn fiel; o core SH-2 segue parcial.


## Avanços adicionais em VDP1 e validação da v0.46b — 2026-10-15
- A execução `38718489736` terminou com sucesso em todos os jobs relevantes: testes host, compilação Xenon ELF32 e empacotamento XEX. O pipeline gerou o artefato `default.xex`.
- VDP1: endurecido o recorte de retângulos para somar coordenadas em `long long`, evitando overflow de inteiros com entradas extremas.
- VDP1: cálculos de interseção de linhas passaram a promover os operandos antes da subtração; vértices de triângulos são limitados a uma área de guarda antes da aritmética de arestas.
- Adicionados testes com coordenadas `INT_MIN`/`INT_MAX` para retângulos, linhas e triângulos.
- Commits: `076c3fe02b486ab1fa9d8f1b77d74cebc65cce73`, `05ff662a3d4d714e4278f3d3a308a2df4a17c392`, `8368086570f367c4c36e4058d78388ef4678c413`, `1a961c7572242fad2f7229384a66c8933fcccf0b`.
- CI dos novos casos VDP1 ainda está em andamento; não declarar v0.46b validada antes dos jobs terminarem com sucesso.
- VDP1 continua sendo rasterizador experimental de framebuffer, não uma implementação fiel completa do hardware Saturn.


## Desenvolvimento adicional: VDP1 e SCU/DMA — 2026-10-15
- CI revelou uma falha em `test_vdp1_renderer` com linha de coordenadas extremas: o cálculo de diferenças de coordenadas ainda podia transbordar antes da promoção para 64 bits.
- Corrigida a aritmética de interseção de triângulos e substituído o recorte de linhas por clipping paramétrico antes da rasterização inteira, limitando os cálculos de Bresenham ao framebuffer.
- Acrescentados testes para linha diagonal e horizontal com coordenadas `INT_MIN`/`INT_MAX`. Os novos runs ainda estavam em andamento na última consulta.
- Criado o protótipo de três canais `ScuDma`, com configuração de origem/destino/quantidade, transferências limitadas por orçamento de bytes, estado busy/completed e reset.
- Criada a suíte `tests/test_scu_dma.cpp` e integrada ao runner de regressões.
- Commits DMA: `cb66e51bb920bb318415cbc01b8c8a7b4eb5b5c4`, `dac391c371f90df8fab1209a7850a0016e89f336`, `b3e50e3ee72462841c6b89bd206fa66422d8bebd`, `5e244a2b33a3594727fb5d1e687820e9a6010a4b`.
- Limitação: `ScuDma` é um modelo genérico experimental de cópia de bytes, não um controlador SCU Saturn fiel com registradores, modos de transferência, arbitragem ou IRQ reais.
- Versão oficial permanece v0.45b; a v0.46b não deve ser promovida até os testes host, compilação Xenon ELF32 e empacotamento XEX passarem na mesma revisão.


## Seis rodadas consecutivas de avanço — 2026-10-15 (candidata v0.46b)

### Rodada 1 — MMIO com larguras reais de acesso
- Adicionados `read16/read32` e `write16/write32` big-endian ao banco de registradores.
- Escritas largas verificam todos os bytes antes de modificar o banco, para rejeitar acessos que cruzam registradores read-only ou áreas não mapeadas.
- Commits: `072ed0e08df827cc1a9d7d8a5a678c26c00f2331`, `dc21f4f7140cb9dcb64a9237ca4e37ceb8ff393f`, `37560c51a2dcb21d9cef09a710716ce8419f72bc`.

### Rodada 2 — limites de endereço MMIO
- Leituras de 16/32 bits que atravessam o limite de 32 bits retornam valores abertos (`0xFFFF`/`0xFFFFFFFF`) sem envolver o endereço para zero.
- Commits: `6d0e599581d0f1c3e06246f6b6ba9e5ce898b364`, `5be69081603934730eb0bb75c0e7b466dbaca30d`.

### Rodada 3 — DMA seguro contra overflow
- A configuração do protótipo SCU/DMA rejeita faixas de origem ou destino que ultrapassem o espaço de endereços de 32 bits.
- Commits: `957f8cd5f4f6bbbd7c629568055df0a5379a8a1f`, `50a6bac97a0b0fd2cf9ffd270cc6b0eaaede66a3`.

### Rodada 4 — rolagem por camada no VDP2 experimental
- Adicionada configuração de rolagem horizontal/vertical independente por camada, com coordenadas negativas e wrap de framebuffer.
- Commits: `0b13ab61d9844698f29a0b8725afddd08aaa3ab3`, `9458bc96ac20fd6b96786700baad7a2ce9a13c23`, `12d503f8bd38a7e1837621691adc9ca5f563d84b`.

### Rodada 5 — testes de integração
- Ampliados os testes MMIO para endianness, rejeição atômica e limite de endereços; testes DMA para faixas que transbordam; testes VDP2 para rolagem positiva, negativa e wrap.
- Os testes estão ligados ao runner `tests/run-sh2-core-tests.sh`; a execução dos commits mais recentes ainda está pendente no CI.

### Rodada 6 — consolidação e validação
- Todos os trabalhos acima são parte da candidata v0.46b, não uma nova versão oficial. Não promover a versão antes de confirmar regressões host, build Xenon ELF32 e empacotamento XEX na revisão integrada.
- Os dispositivos continuam protótipos: o banco MMIO não possui handlers específicos completos; DMA não implementa o protocolo real do SCU; VDP2 não reproduz integralmente planos, registradores e efeitos do hardware.
- Não há comprovação de execução em Xbox 360 físico nem compatibilidade com jogos comerciais.


## Lote de desenvolvimento integrado — sequência de 50 passagens solicitada — 2026-10-15
Para manter alterações verificáveis, a solicitação de 50 passagens foi executada como um lote de mudanças pequenas e revisáveis, em vez de 50 incrementos de versão artificiais.
- Barramento SH-2: acessos auxiliares de 16/32 bits agora recusam leituras/escritas que transbordem o endereço de 32 bits, evitando wrap para zero. Commits `907894004f748fe30c22cf5e3609b7bcf074a6d3`, `c68879cab1b28a4d642f3b242b967402dc79a746`.
- SCU/DMA experimental: serviço round-robin entre canais sob orçamento de bytes, mantendo a quantidade total transferida limitada ao orçamento. Commits `0f1191b68828ea52dde9fe76a1989f465e573657`, `a92d9274479b6b1eca96b32275b99462bd97a3f2`, `93b8fe7d4a96cecb9bf91d03d84c38b7b866d7a8`.
- DMA: adicionados testes de equidade entre dois canais ativos e orçamento zero; o teste foi isolado com reset para não depender do estado anterior. Commits `060bb5f0316fe13fdd430102e9c07995348f9821`, `49803590f72bc9fbc6c60869a8c4542ec4c46c5e`.
- VDP2 experimental: teste adicional para deslocamentos extremos `INT_MIN/INT_MAX`, confirmando que a API normaliza o scroll antes de compor. Commit `7b8c079ea6f913c41ac415fdba1aee3fabbb84d9`.
- CI: execuções anteriores das alterações de MMIO e VDP2 terminaram com sucesso. A nova revisão de barramento/DMA/VDP2 ainda precisa de seu próprio resultado completo de testes, compilação Xenon e empacotamento XEX.
- A versão oficial permanece v0.45b; v0.46b é candidata e não deve ser promovida sem validação integrada. Sem alegação de execução em hardware ou compatibilidade de jogos comerciais.

## Lote de 20 avanços solicitados — testes de regressão adicionais (2026-10-15)

Para evitar saltos artificiais de versão, os avanços foram agrupados em alterações pequenas e rastreáveis, mantendo a versão oficial **v0.45b** e a candidata **v0.46b**.

1. Revalidada a execução integrada do workflow no commit `7b8c079ea6f913c41ac415fdba1aee3fabbb84d9`: testes host, build Xenon ELF32 e empacotamento `default.xex` passaram.
2. Investigada a regressão intermediária do escalonador DMA: o teste antigo falhava por dependência do estado entre cenários; o isolamento com `reset()` está no commit `49803590f72bc9fbc6c60869a8c4542ec4c46c5e`, cuja execução posterior passou.
3. Adicionado teste do VDP2 para prioridades iguais, ordenação estável por índice de camada e máscara RGB555, incluindo o bit 15 do backdrop. Commit `2efd312167a15fb6391030c91966f560112da99b`.
4. Adicionado teste DMA em que um canal curto deve concluir enquanto outro canal mantém uma transferência maior ativa. Commit `2c026304addf79f249e98c9dd21f79f041825067`.
5. Mantida a distinção entre build e funcionamento real: o XEX empacotado é um bootstrap experimental, não demonstra ainda boot do Saturn nem execução de jogos comerciais.
6. O commit `2c026304addf79f249e98c9dd21f79f041825067` e os testes de prioridade VDP2 precisam de uma nova execução integrada de CI; não declarar estes testes novos como aprovados até o resultado.
7. As etapas restantes deste lote continuam focadas em ampliar regressões determinísticas e preparar integração, sem declarar concluídos CPU SH-2, VDP1/VDP2, SCU, CD Block, SCSP ou SMPC.

## Lote de 20 frentes — continuação de 2026-10-15

A versão oficial permanece **v0.45b**; **v0.46b** continua candidata até a validação integrada.

1. Conferido o estado recente do GitHub Actions.
2. Confirmado que o teste de VDP2 para prioridades iguais e máscara RGB555 passou no run associado ao commit `2efd312167a15fb6391030c91966f560112da99b`.
3. Investigada a falha intermediária de DMA usando o log do host; a expectativa anterior dependia indevidamente do estado/ordem de canais.
4. Adicionado teste para conclusão de canal DMA curto durante uma transferência longa — commit `70e0958a786c74fcbbda872f746748ac8caa0a84`.
5. Adicionado teste de justiça entre três canais DMA — commit `395a5be1b7304b5bf838bedf6941d5c8b0884aca`.
6. Preservado o orçamento máximo de bytes por chamada do escalonador.
7. Preservados os testes de faixas de origem/destino que transbordam 32 bits.
8. Preservado o teste de transferência de tamanho zero.
9. Preservado o teste de reset e isolamento entre transferências.
10. Adicionada regressão para escrita de 16 bits no fim do espaço de endereços.
11. Adicionada regressão para escrita de 32 bits no fim do espaço de endereços.
12. Verificado que escritas largas inválidas não devem envolver o endereço para zero — commit `f3fc3e62c5ac065dd99b0997dca5a079a14bd7f4`.
13. Ampliados os testes MMIO para rejeição de escrita de 16 bits em faixa inválida.
14. Ampliados os testes MMIO para rejeição de escrita de 32 bits com overflow — commit `0855b57c68a9bfad47cf926b65d4da6e0267ebf1`.
15. Mantida a validação de todos os bytes e permissões antes de escritas MMIO largas.
16. Mantida a ordem big-endian para acessos de 16 e 32 bits.
17. Preservados os testes de prioridade estável no compositor VDP2.
18. Preservados os testes de scroll positivo, negativo e extremo no VDP2.
19. Não declarados completos CPU SH-2, VDP1/VDP2, SCU, CD Block, SCSP ou SMPC; continuam protótipos parciais.
20. As alterações mais recentes de DMA e barramento ainda precisam de CI integrado próprio antes de serem consideradas aprovadas.

**Critério de aceitação:** testes host, build Xenon ELF32 e empacotamento `default.xex` devem passar na mesma execução integrada. Isso ainda não prova boot em hardware nem compatibilidade de jogos.

## Lote de 50 etapas — continuação SH-2 / integração (2026-10-15)

Este lote registra uma alteração real no repositório e 50 itens de trabalho/verificação. Itens descritos como “pendente” são tarefas de acompanhamento, não código já concluído. A versão oficial permanece **v0.45b**; **v0.46b** segue candidata até a CI integrada passar.

1. Reconsultado o estado de GitHub Actions para a base anterior.
2. Confirmado sucesso do workflow para o teste MMIO no commit `0855b57`.
3. Confirmado sucesso do workflow de justiça DMA para três canais no commit `395a5be`.
4. Confirmado sucesso do workflow de overflow de escrita do barramento no commit `f3fc3e6`.
5. Confirmado sucesso do workflow de conclusão de canal DMA curto no commit `70e0958`.
6. Mantido o critério de não promover versão apenas com base em testes isolados.
7. Reexaminado o decodificador SH-2 antes de ampliar a cobertura de instruções.
8. Identificada a ausência de handlers explícitos para DIV0U e DIV0S.
9. Implementado handler de DIV0U para limpar Q, M e T no SR.
10. Implementado handler de DIV0S para inicializar Q/M a partir dos sinais dos operandos e T = Q XOR M.
11. Preservados os bits não relacionados do SR nas duas operações.
12. Adicionados testes de DIV0U para limpeza de flags.
13. Adicionado teste de DIV0S com operandos de sinais diferentes.
14. Adicionado teste de DIV0S com ambos os operandos negativos.
15. Commit do núcleo: `03ce8f79c2789b3affbb3f481625992707456ca8`.
16. Commit dos testes: `a5b3b2a799698c912ea454325d676bb300d9e819`.
17. A CI dos dois commits novos ainda precisa ser consultada; os testes não são declarados aprovados até o resultado.
18. Verificar na CI a compilação do conjunto de testes SH-2 com `-Wall -Wextra -Werror -pedantic`.
19. Verificar a suíte de acessos à memória SH-2.
20. Verificar a suíte de aritmética e flags.
21. Verificar a suíte de delay slots.
22. Verificar a suíte de fluxo de controle.
23. Verificar a integração SH-2 com o barramento Saturn.
24. Verificar o runner e o loop principal.
25. Verificar os testes do mapa físico de memória.
26. Verificar os testes MMIO e os acessos big-endian.
27. Verificar os testes do rasterizador VDP1.
28. Verificar os testes da lista de comandos VDP1.
29. Verificar os testes do compositor VDP2.
30. Verificar os testes de DMA/SCU.
31. Manter a verificação de endereços que transbordam o espaço de 32 bits.
32. Manter a verificação de escritas MMIO largas atômicas em faixas inválidas.
33. Manter a verificação de rotação justa entre canais DMA.
34. Manter a verificação de que canal curto termina enquanto outro continua ativo.
35. Manter o teste de orçamento DMA zero.
36. Manter a cobertura de prioridade estável do VDP2.
37. Manter a cobertura de scroll positivo, negativo e extremo.
38. Próximo passo SH-2: implementar DIV1 com tabela de casos e testes de referência.
39. Próximo passo SH-2: auditar MAC.L/MAC.W e as condições de saturação.
40. Próximo passo SH-2: confirmar TAS.B e operações de bits na memória.
41. Próximo passo SH-2: revisar STC/LDC/STS/LDS e seus efeitos em SR.
42. Próximo passo SH-2: desenhar o modelo de exceção para TRAPA/RTE antes de integrar handlers.
43. Próximo passo: ligar os canais DMA ao barramento de memória real e mapear efeitos colaterais dos dispositivos.
44. Próximo passo: substituir armazenamento MMIO genérico por registradores específicos de cada dispositivo.
45. Próximo passo: conectar VDP1/VDP2 ao loop de quadro e à saída de vídeo.
46. Próximo passo: integrar temporização e interrupções com o scheduler.
47. Próximo passo: avançar o CD Block além do protótipo de registradores.
48. Próximo passo: avançar o SCSP além do protótipo de registradores.
49. Próximo passo: validar SMPC e leitura real de controles.
50. Gate de lançamento: obter sucesso integrado de testes host + ELF32 Xenon + empacotamento `default.xex`; boot em Xbox 360 físico e compatibilidade de jogos continuam sem comprovação.

**Estado honesto:** a implementação de DIV0U/DIV0S e seus testes foi gravada no GitHub. A validação automática posterior ainda é necessária. Nenhuma alegação de boot físico ou compatibilidade comercial é feita.


## Continuação — cobertura de flags DIV0 (15/10/2026)

- Acrescentados casos de teste para DIV0U verificar limpeza de Q/M/T e preservação de bits não relacionados de SR.
- Ampliada a matriz DIV0S para as quatro combinações de sinais de Rn/Rm, incluindo a relação Q/M/T.
- Corrigido o opcode de teste para representar explicitamente DIV0S R2,R1.
- Commits: `bb190c30847f02a8276edf1667a7df6a3eb48df3` e correção `fa15b7282140d35b735d15372022660ec7379b1d`.
- Próxima verificação obrigatória: resultado do GitHub Actions para o commit mais recente; não declarar testes aprovados antes da conclusão.
- A versão oficial permanece v0.45b; v0.46b continua candidata até passar o gate integrado.
