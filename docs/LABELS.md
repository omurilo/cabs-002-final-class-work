# GitHub Issue Labels Configuration

This document defines the labels used for issue tracking in this project.

## Milestone Labels

- `milestone-1`: Planejamento e design (Arquitetura Base)
- `milestone-2`: Core das estruturas (modelo puro)
- `milestone-3`: Sistema de animação e visualização base
- `milestone-4`: Gravação/Reprodução (modelo + JSON)
- `milestone-5`: Persistência (frames + vídeo)
- `milestone-6`: Concorrência e cancelamento
- `milestone-7`: Input e HUD
- `milestone-8`: Testes unitários iniciais
- `milestone-9`: Build system e CI
- `milestone-10`: Documentação principal
- `milestone-11`: Refinamentos (temas, layout avançado)
- `milestone-12`: Otimizações e sanitizers
- `milestone-13`: Release e roadmap futuro

## Type Labels

- `bug`: Something isn't working
- `enhancement`: New feature or request
- `documentation`: Improvements or additions to documentation
- `refactor`: Code refactoring
- `test`: Testing related
- `performance`: Performance improvements
- `security`: Security related issues

## Priority Labels

- `priority-critical`: Must be fixed immediately
- `priority-high`: Should be addressed soon
- `priority-medium`: Normal priority
- `priority-low`: Nice to have

## Status Labels

- `status-blocked`: Blocked by another issue
- `status-in-progress`: Currently being worked on
- `status-needs-review`: Needs code review
- `status-needs-testing`: Needs testing

## Component Labels

- `component-core`: Core data structures
- `component-animation`: Animation engine
- `component-visualization`: Rendering system
- `component-persistence`: Serialization/storage
- `component-concurrency`: Threading/parallelism
- `component-input`: Input and HUD
- `component-build`: Build system
- `component-ci`: CI/CD

## Special Labels

- `good-first-issue`: Good for newcomers
- `help-wanted`: Extra attention needed
- `question`: Further information requested
- `wontfix`: This will not be worked on
- `duplicate`: This issue already exists
- `invalid`: This doesn't seem right

## Phase Labels

- `phase-1-foundation`: Foundation phase (Milestones 1-3)
- `phase-2-features`: Features phase (Milestones 4-7)
- `phase-3-quality`: Quality phase (Milestones 8-10)
- `phase-4-polish`: Polish phase (Milestones 11-13)

## Usage Guidelines

1. Every issue should have at least one type label
2. Milestone issues should have the corresponding milestone label
3. Priority should be assigned to bugs and critical enhancements
4. Component labels help organize work by area
5. Good first issues should be clearly documented and scoped

## Creating Labels in GitHub

You can create these labels using the GitHub CLI:

```bash
gh label create "milestone-1" --color "0e8a16" --description "Planejamento e design"
gh label create "bug" --color "d73a4a" --description "Something isn't working"
gh label create "enhancement" --color "a2eeef" --description "New feature or request"
# ... etc
```

Or use the GitHub web interface: Settings → Labels
