# Shining Saturn 360

Emulador de Sega Saturn para Xbox 360 (RGH/JTAG).

## Estado técnico — v0.41b

## Versão publicada no repositório

A documentação da versão atual está em [VERSION_v0.41b.md](VERSION_v0.41b.md). Ela registra o estado experimental, a execução de CI e o artefato de bootstrap disponível; não implica suporte comprovado a jogos nem inicialização em hardware real.

- Testes de regressão no Ubuntu: aprovados na execução [#26](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38210909197).
- Toolchain libXenon: verificada.
- Auditoria ELF32 big-endian PowerPC: aprovada; ponto de entrada `0x00005500` está dentro de um segmento `PT_LOAD` executável; 5 segmentos carregáveis encontrados.
- Conversão para `default.xex`: concluída.
- Verificação estrutural XEX2: aprovada; base do conteúdo `0x00001000`, imagem de 589.824 bytes, arquivo de 593.920 bytes, 8 cabeçalhos opcionais.
- Verificação de permissões de páginas: o empacotador reportou 4 páginas `CODE` seguidas de 5 páginas `RWDATA`, sem aviso de compartilhamento de páginas.
- [Obter o artefato `ShiningSaturn360-default-XEX`](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38210909197/artifacts/11728828108). Inclui `default.xex`, checksums e log de empacotamento; retenção até 26 de outubro de 2026.
- SHA-256 do `default.xex`: `87e448b99226a43fea5acfea7e2abd407ce308f8c6f839dec692d1cc39204b1c`.

## Núcleo SH-2 experimental (em desenvolvimento)

Foi adicionada uma base independente de interpretador SH-2 em `src/saturn/sh2_core.hpp` e `src/saturn/sh2_core.cpp`, com um teste compilável em `tests/test_sh2_core.cpp`. A implementação cobre um subconjunto experimental de instruções: movimentação, aritmética e lógica, acessos básicos à memória, operações de deslocamento, testes de condição e desvios condicionais sem atraso e com slot de atraso, chamadas relativas ao PC e saltos indiretos. Foram acrescentados modos de memória com pós-incremento/pré-decremento, acessos relativos ao PC e com deslocamento, comparações signed/unsigned, operações com carry/borrow, rotações, multiplicação de 32/64 bits, SWAP, EXTU/EXTS, XTRCT e novas formas de desvio condicional/relativo. A cobertura é parcial e ainda exige validação e expansão para a arquitetura SH-2 completa. Os testes exercitam instruções imediatas, registradores, endianness e tratamento explícito de opcode não suportado. Não é uma CPU SH-2 completa.

Os testes são executados no GitHub Actions pelo script `tests/run-sh2-core-tests.sh`. Este núcleo ainda não está integrado ao bootstrap `default.xex` e não implementa os processadores, VDPs, áudio, temporização, CD-ROM ou demais subsistemas necessários para executar jogos de Saturn.

## Barramento de memória experimental

Foi adicionada uma camada genérica em `src/saturn/memory_bus.hpp` e `src/saturn/memory_bus.cpp` para mapear regiões de RAM e ROM, validar sobreposições, permitir gravação somente em RAM e definir leitura `0xFF` para endereços não mapeados. Os testes correspondentes ficam em `tests/test_memory_bus.cpp` e entram no script de regressão.

**Limite importante:** isto ainda não é o mapa real de memória do Sega Saturn. Não há ainda decodificação dos endereços dos dois SH-2, áreas de trabalho compartilhadas, registradores de VDP1/VDP2, SCSP, SMPC, barramento de CD-ROM ou arbitragem/timing. A camada genérica é uma base para implementar esses dispositivos, não um subsistema Saturn completo.

## Limitações atuais

O `default.xex` é um bootstrap técnico. A geração do arquivo e as verificações estruturais não comprovam que ele inicializa num Xbox 360 real, nem que o núcleo de emulação Sega Saturn esteja funcional ou execute jogos. A inicialização no console ainda não foi testada.

A versão publicada continua `v0.41b`; as melhorias do núcleo SH-2 são alterações de desenvolvimento posteriores e ainda não estão integradas ao `default.xex`. A versão não representa um emulador funcional completo.

## Compilação na nuvem

Abra o workflow **Shining Saturn 360 - Xenon audit and build**:
https://github.com/MeiKinosaki/Shining-Saturn-360/actions


## Biblioteca de jogos — base experimental

Foi adicionada uma biblioteca de metadados em `src/saturn/game_library.hpp/.cpp`. Ela oferece uma coleção de jogos, associação de caminho e capa, estado favorito, filtros por texto/gênero/desenvolvedora/região/ano, filtro de jogos com ou sem dados/estados e ordenação por título ou ano. Os diretórios padrão configuráveis são Roms, Favorites, SaveState, SaveData, Covers e Config.

Os testes ficam em `tests/test_game_library.cpp` e são executados pelo script de regressão. **Limites:** ainda não há varredura real do disco do Xbox 360, interface gráfica conectada a esta biblioteca, leitura/validação de CUE/ISO, persistência da lista, criação de capas ou sistema funcional de save state/save data. Os filtros de save usam metadados fornecidos à biblioteca; eles não inspecionam o armazenamento automaticamente.


## Música do menu — controlador experimental

O módulo `src/saturn/menu_music.hpp/.cpp` define um controlador independente de plataforma para faixa de menu: caminho padrão `Hdd1:\\ShiningSaturn360\\Music\\MenuTheme.wav`, ativar/desativar, repetição, volume limitado a 0–100%, pausa, retomada e parada. O backend de áudio específico do Xbox 360 ainda precisa ser implementado e ligado ao ciclo de vida da interface; portanto, este módulo sozinho não toca áudio no XEX. Coloque apenas áudio que você tenha direito de usar na pasta Music. Testes unitários com backend simulado ficam em `tests/test_menu_music.cpp`.
