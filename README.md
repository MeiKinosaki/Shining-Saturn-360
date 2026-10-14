# Shining Saturn 360

Emulador de Sega Saturn para Xbox 360 (RGH/JTAG).

## Estado técnico — v0.44b

## Versão publicada no repositório

A documentação da versão atual está em [VERSION_v0.43b.md](VERSION_v0.43b.md). Ela registra o estado experimental, a execução de CI e o artefato de bootstrap disponível; não implica suporte comprovado a jogos nem inicialização em hardware real.

- Testes de regressão no Ubuntu: aprovados na execução [mais recente validada](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38366824872) (compilação Xenon ELF32, testes do núcleo SH-2 e empacotamento XEX). A aprovação do CI não comprova inicialização em hardware real.
- Toolchain libXenon: verificada.
- Auditoria ELF32 big-endian PowerPC: aprovada; ponto de entrada `0x00005500` está dentro de um segmento `PT_LOAD` executável; 5 segmentos carregáveis encontrados.
- Conversão para `default.xex`: concluída.
- Verificação estrutural XEX2: aprovada; base do conteúdo `0x00001000`, imagem de 589.824 bytes, arquivo de 593.920 bytes, 8 cabeçalhos opcionais.
- Verificação de permissões de páginas: o empacotador reportou 4 páginas `CODE` seguidas de 5 páginas `RWDATA`, sem aviso de compartilhamento de páginas.
- [Obter o artefato `ShiningSaturn360-default-XEX`](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38210909197/artifacts/11728828108). Inclui `default.xex`, checksums e log de empacotamento; retenção até 26 de outubro de 2026.
- SHA-256 do `default.xex`: `87e448b99226a43fea5acfea7e2abd407ce308f8c6f839dec692d1cc39204b1c`.

## Teste imediato no Xbox 360 (bootstrap experimental)

1. Baixe o artefato **ShiningSaturn360-default-XEX** da execução [associada ao commit atual](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38370373297).
2. Extraia o ZIP no computador e confira o arquivo `default.xex` e o arquivo de checksum incluído.
3. Em um Xbox 360 compatível com homebrew (por exemplo, RGH/JTAG), copie a pasta para `Hdd1:\\Games\\ShiningSaturn360\\` ou para uma pasta em uma unidade USB.
4. No dashboard/homebrew file manager, abra a pasta e tente iniciar `default.xex`.
5. Registre o resultado: se abrir, se ficar em tela preta, se voltar ao dashboard ou se apresentar erro. Isso é necessário para diagnosticar o primeiro teste em hardware real.

**Importante:** esta compilação é um bootstrap técnico e ainda não é um emulador de Saturn jogável. O CI confirma compilação e empacotamento, não o funcionamento no console. O núcleo SH-2 experimental não está ligado ao bootstrap e os subsistemas de vídeo, áudio e CD-ROM ainda não permitem iniciar jogos. Não atualize a NAND/firmware para este teste; basta tentar executar o XEX existente num ambiente homebrew já funcional.

## Marco v0.44b — ciclo principal experimental

O `src/saturn/saturn_main_loop.hpp/.cpp` conecta o SH-2 Runner ao barramento Saturn e executa orçamentos limitados por quadro. Os testes de integração e MMIO passaram no GitHub Actions: [ciclo principal](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38503506652) e [MMIO](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38503177536). Isso valida somente os testes automatizados; não comprova boot no console nem execução de jogos. A CPU segue parcial, os registradores MMIO ainda não possuem handlers reais dos dispositivos, e VDPs, CD-ROM e áudio continuam pendentes.

## Núcleo SH-2 experimental (em desenvolvimento)

Foi adicionada uma base independente de interpretador SH-2 em `src/saturn/sh2_core.hpp` e `src/saturn/sh2_core.cpp`, com um teste compilável em `tests/test_sh2_core.cpp`. A implementação cobre um subconjunto experimental de instruções: movimentação, aritmética e lógica, acessos básicos à memória, operações de deslocamento, testes de condição e desvios condicionais sem atraso e com slot de atraso, chamadas relativas ao PC e saltos indiretos. Foram acrescentados modos de memória com pós-incremento/pré-decremento, acessos relativos ao PC e com deslocamento, comparações signed/unsigned, operações com carry/borrow, rotações, multiplicação de 32/64 bits, SWAP, EXTU/EXTS, XTRCT e novas formas de desvio condicional/relativo. A cobertura é parcial e ainda exige validação e expansão para a arquitetura SH-2 completa. Os testes exercitam instruções imediatas, registradores, endianness e tratamento explícito de opcode não suportado. Não é uma CPU SH-2 completa.

Os testes são executados no GitHub Actions pelo script `tests/run-sh2-core-tests.sh`. Este núcleo ainda não está integrado ao bootstrap `default.xex` e não implementa os processadores, VDPs, áudio, temporização, CD-ROM ou demais subsistemas necessários para executar jogos de Saturn.

## Barramento de memória experimental

Foi adicionada uma camada genérica em `src/saturn/memory_bus.hpp` e `src/saturn/memory_bus.cpp` para mapear regiões de RAM e ROM, validar sobreposições, permitir gravação somente em RAM e definir leitura `0xFF` para endereços não mapeados. Os testes correspondentes ficam em `tests/test_memory_bus.cpp` e entram no script de regressão.

**Limite importante:** isto ainda não é o mapa real de memória do Sega Saturn. Não há ainda decodificação dos endereços dos dois SH-2, áreas de trabalho compartilhadas, registradores de VDP1/VDP2, SCSP, SMPC, barramento de CD-ROM ou arbitragem/timing. A camada genérica é uma base para implementar esses dispositivos, não um subsistema Saturn completo.

## Limitações atuais

O `default.xex` é um bootstrap técnico. A geração do arquivo e as verificações estruturais não comprovam que ele inicializa num Xbox 360 real, nem que o núcleo de emulação Sega Saturn esteja funcional ou execute jogos. A inicialização no console ainda não foi testada.

A versão de desenvolvimento passa a `v0.43b`, para registrar o adaptador inicial de memória. O núcleo SH-2 ainda não está integrado ao `default.xex`. A versão não representa um emulador funcional completo.

## Compilação na nuvem

Abra o workflow **Shining Saturn 360 - Xenon audit and build**:
https://github.com/MeiKinosaki/Shining-Saturn-360/actions


## Biblioteca de jogos — base experimental

Foi adicionada uma biblioteca de metadados em `src/saturn/game_library.hpp/.cpp`. Ela oferece uma coleção de jogos, associação de caminho e capa, estado favorito, filtros por texto/gênero/desenvolvedora/região/ano, filtro de jogos com ou sem dados/estados e ordenação por título ou ano. Os diretórios padrão configuráveis são Roms, Favorites, SaveState, SaveData, Covers e Config.

Os testes ficam em `tests/test_game_library.cpp` e são executados pelo script de regressão. **Limites:** ainda não há varredura real do disco do Xbox 360, interface gráfica conectada a esta biblioteca, leitura/validação de CUE/ISO, persistência da lista, criação de capas ou sistema funcional de save state/save data. Os filtros de save usam metadados fornecidos à biblioteca; eles não inspecionam o armazenamento automaticamente.


## Música do menu — controlador experimental

O módulo `src/saturn/menu_music.hpp/.cpp` define um controlador independente de plataforma para faixa de menu: caminho padrão `Hdd1:\\ShiningSaturn360\\Music\\MenuTheme.wav`, ativar/desativar, repetição, volume limitado a 0–100%, pausa, retomada e parada. O backend de áudio específico do Xbox 360 ainda precisa ser implementado e ligado ao ciclo de vida da interface; portanto, este módulo sozinho não toca áudio no XEX. Coloque apenas áudio que você tenha direito de usar na pasta Music. Testes unitários com backend simulado ficam em `tests/test_menu_music.cpp`.

## Descrição do emulador

O **Shining Saturn 360** é um projeto experimental que pretende emular o Sega Saturn no Xbox 360 com RGH/JTAG. O objetivo é evoluir de um bootstrap técnico para um emulador com núcleo SH-2, vídeo VDP1/VDP2, áudio SCSP, CD-ROM, controles e uma biblioteca visual de jogos. **No estado atual, ele ainda não executa jogos de Saturn de forma comprovada.** O núcleo SH-2, o barramento, a biblioteca e o scanner de ROMs são bases experimentais e não estão todos integrados ao executável.

## Controles

Não existe ainda um mapeamento funcional de controles implementado no emulador. A tabela abaixo é uma **proposta para a futura interface**, não uma função disponível na versão atual:

| Controle Xbox 360 | Ação planejada |
|---|---|
| D-pad | Navegar pelos menus |
| A | Confirmar / abrir jogo |
| B | Voltar |
| X | Opções do jogo |
| Y | Marcar/desmarcar favorito |
| LB / RB | Mudar categoria ou página |
| Start | Abrir menu |
| Back | Informações do jogo |

O mapeamento dos botões do controle para um gamepad Sega Saturn ainda precisa ser implementado e testado.

## Settings (configurações)

Ainda não há uma tela de configurações funcional conectada ao executável. Estas são as opções planejadas:

- **Interface:** tamanho das capas, ordenação e filtros da biblioteca.
- **Vídeo:** proporção da imagem, escala e filtros gráficos, quando o VDP estiver implementado.
- **Desempenho:** opções de sincronização e limites de velocidade, após existir temporização funcional.
- **Áudio:** ativar/desativar música do menu e volume; o controlador experimental existe, mas o backend de áudio Xbox 360 ainda está pendente.
- **Pastas:** diretórios de ROMs, capas, configurações e saves.
- **Controles:** configuração dos botões do Xbox 360 para o controle Saturn.

Não considere essas opções disponíveis no executável atual até que sejam integradas à interface.

## ROMs e capas

Estrutura de pastas prevista para instalação no disco rígido:

```text
Hdd1:\ShiningSaturn360\
├── Roms\       # Jogos que você possui e tem direito de usar
├── Covers\     # Capas com o mesmo nome-base do arquivo do jogo
├── Favorites\  # Reservado para futura persistência de favoritos
├── SaveData\   # Reservado para dados de jogo
├── SaveState\  # Reservado para estados instantâneos
├── Config\     # Reservado para configurações
└── Music\      # Áudio que você tenha direito de usar
```

O scanner experimental reconhece as extensões .cue, .iso, .chd, .ccd, .bin, .img e .mds e procura capas .png, .jpg, .jpeg ou .bmp com o mesmo nome-base. Ele ainda depende de um adaptador de sistema de arquivos; a leitura real do HDD do Xbox 360 e a integração à interface não estão concluídas. Nem toda extensão listada garante que o formato seja carregável pelo emulador.

## Save Data e Save State / Load State

**Ainda não implementados como funcionalidades de emulação.**

- **Save Data:** diretório reservado em Hdd1:\ShiningSaturn360\SaveData\ para futuros dados persistentes de jogos.
- **Save State:** deverá guardar CPU, memória, registradores de dispositivos e estado de temporização num ponto da execução.
- **Load State:** deverá restaurar esses componentes de forma consistente.

O estado atual não tem os subsistemas completos nem a serialização necessária para criar ou carregar saves reais. Os campos de metadados da biblioteca não equivalem a um sistema de salvamento funcional.

## Como instalar (bootstrap experimental)

> **Aviso:** isto instala apenas o artefato técnico experimental. Não garante que o XEX abra no console nem que execute jogos.

1. Num computador, abra o workflow de compilação: https://github.com/MeiKinosaki/Shining-Saturn-360/actions
2. Selecione uma execução concluída com sucesso e abra **Artifacts**.
3. Baixe o artefato ShiningSaturn360-default-XEX (quando disponível) e extraia o ZIP.
4. Copie default.xex para uma pasta no HDD do Xbox 360 RGH/JTAG, por exemplo Hdd1:\ShiningSaturn360\, usando um método de transferência de arquivos que você já utilize no console.
5. Inicie default.xex por um gerenciador de arquivos compatível.
6. Para os testes de desenvolvimento, confira também a seção Compilação na nuvem e os logs do workflow.

Se o artefato não estiver disponível ou tiver expirado, execute novamente o workflow e baixe o artefato da execução bem-sucedida mais recente. Não use a criação do XEX como prova de compatibilidade; a inicialização em hardware real ainda precisa ser testada.

## Estado das funcionalidades (resumo)

O marco de desenvolvimento atual é **v0.43b**; veja [VERSION_v0.43b.md](VERSION_v0.43b.md). O adaptador inicial liga regiões de RAM/ROM ao barramento genérico, mas ainda não implementa MMIO nem dispositivos Saturn funcionais.

| Recurso | Estado |
|---|---|
| Build de auditoria Xenon / geração do XEX | Pipeline verificado; inicialização no console não confirmada |
| Núcleo SH-2 | Parcial e experimental; não integrado ao XEX |
| Barramento de memória | Adaptador experimental com RAM/ROM mapeadas; MMIO e integração SH-2 pendentes |
| Biblioteca, filtros e favoritos | Base de código; UI e persistência não integradas |
| Scanner de ROMs e associação de capas | Base experimental; adaptador do HDD ainda pendente |
| Controles na interface e em jogos | Planejados, não implementados |
| Settings de vídeo/desempenho | Planejados, não implementados |
| Save Data / Save State / Load State | Não implementados |
| VDP1 / VDP2 / SCSP / CD-ROM / SMPC | Ainda não implementados como subsistemas funcionais integrados |
| Compatibilidade com jogos | Não validada |

## Configurações persistentes — módulo experimental

Foi adicionado `src/saturn/settings.hpp/.cpp`, com leitura e gravação versionada por meio de um adaptador de armazenamento. O formato é texto `chave=valor`, pensado para `Hdd1:\ShiningSaturn360\Config\settings.ini` quando o adaptador de HDD estiver ligado. Inclui escala de interface, filtro gráfico (reserva), preset de desempenho (reserva), VSync (reserva), música do menu, volume e ordenação da biblioteca.

Exemplo do formato:

```ini
version=1
ui_scale=100
video_filter=0
performance_preset=0
vsync=1
menu_music=1
menu_volume=0.65
sort=0
```

Os valores são validados e limitados a intervalos seguros. **Limite atual:** a camada é testável com armazenamento simulado; ainda falta implementar o adaptador de arquivo real para o Xbox 360 e conectar essas opções à interface e aos subsistemas de vídeo/desempenho. A presença das opções no arquivo não significa que filtros gráficos ou VSync estejam funcionais.