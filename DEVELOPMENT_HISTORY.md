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
