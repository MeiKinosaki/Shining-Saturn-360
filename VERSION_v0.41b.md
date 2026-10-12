# Shining Saturn 360 — v0.41b

Este repositório registra o estágio experimental do Shining Saturn 360 para Xbox 360 (RGH/JTAG).

## Versão atual

**v0.41b — auditoria ELF e empacotamento XEX**

- Workflow na nuvem: [Shining Saturn 360 - Xenon audit and build](https://github.com/MeiKinosaki/Shining-Saturn-360/actions).
- Última execução validada para este marco: [execução #26](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38210909197).
- ELF32 PowerPC big-endian auditado: o ponto de entrada informado está dentro de um segmento carregável executável.
- Empacotamento `default.xex` e verificações estruturais XEX2 passaram na execução #26.
- O artefato da execução #26 pode ser obtido na [página de artefatos](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38210909197/artifacts/11728828108), sujeito ao prazo de retenção do GitHub Actions.

## Importante: conteúdo real desta versão

A auditoria, os testes de regressão e a geração de um XEX demonstram somente que o bootstrap experimental foi compilado e empacotado. **Não** demonstram inicialização em hardware Xbox 360 real nem execução de jogos de Sega Saturn.

Os arquivos de código-fonte legados estão preservados nos arquivos ZIP versionados abaixo. Este commit atualiza os metadados/documentação da versão e não afirma que o conteúdo legado foi substituído por um núcleo completo de emulação.

## Arquivos de origem preservados

- `ShiningSaturn360_v0.39b_repo_update.zip`
- `ShiningSaturn360_v0.39b_source_audit_patch.zip`
- `ShiningSaturn360_v0_25b_source.zip`
- `ShiningSaturn360_v0_26b_cmp_eq_regression.zip`

## Próximo marco

Validar a inicialização do bootstrap em um ambiente compatível e, posteriormente, no hardware alvo. Só depois devem ser declarados suporte real de console, compatibilidade de jogos ou nova versão funcional.
