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

## Limitações atuais

O `default.xex` é um bootstrap técnico. A geração do arquivo e as verificações estruturais não comprovam que ele inicializa num Xbox 360 real, nem que o núcleo de emulação Sega Saturn esteja funcional ou execute jogos. A inicialização no console ainda não foi testada.

A versão publicada continua `v0.41b`; as melhorias do núcleo SH-2 são alterações de desenvolvimento posteriores e ainda não estão integradas ao `default.xex`. A versão não representa um emulador funcional completo.

## Compilação na nuvem

Abra o workflow **Shining Saturn 360 - Xenon audit and build**:
https://github.com/MeiKinosaki/Shining-Saturn-360/actions
