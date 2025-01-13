# beeps ChangeLog


## [v0.3.2] - 2025-01-14

- Update workflow files
- Set minumum version for runtime dependency


## [v0.3.1] - 2025-01-13

- Add Sequencer class
- Add 'triangle' type to Oscillator
- Add LICENSE file
- Rename: ADSR -> Envelope


## [v0.3] - 2024-07-06

- Support Windows


## [v0.2.1] - 2024-07-05

- Do not delete OpenAL objects after calling OpenAL_fin()
- Do not redefine fin!() methods, they are no longer needed
- Skip 'test_play_end_then_stop' on GitHub Actions
- Update workflows for test
- Update to actions/checkout@v4
- Fix 'github_actions?'


## [v0.2] - 2024-03-14

- Change the super class for exception class from RuntimeError to StandardError


## [v0.1.46] - 2024-02-07

- Fix compile warnings


## [v0.1.45] - 2024-01-08

- Update dependencies


## [v0.1.44] - 2023-12-09

- Trigger github actions on all pull_request


## [v0.1.43] - 2023-11-09

- Use Gemfile to install gems for development instead of add_development_dependency in gemspec


## [v0.1.42] - 2023-10-25

- Add '#include <assert.h>' to Fix compile errors


## [v0.1.41] - 2023-06-27

- Add SoundPlayer#state()
- Delete SoundPlayer#is_playing(), is_paused(), and is_stopped()
- NONE -> TYPE_NONE


## [v0.1.40] - 2023-05-29

- Update dependencies


## [v0.1.39] - 2023-05-29

- Update dependencies


## [v0.1.38] - 2023-05-27

- required_ruby_version >= 3.0.0
- Add spec.license


## [v0.1.37] - 2023-05-18

- Update dependencies


## [v0.1.36] - 2023-05-08

- Update dependencies


## [v0.1.35] - 2023-04-30

- Place the save() method next to load()


## [v0.1.34] - 2023-04-25

- Clear mic streams on exit
- Fix compile errors on assert lines


## [v0.1.33] - 2023-04-22

- Stream playback is now supported.
- Pause, resume, and stop after playback, as well as methods to check the status of the playback.
- Support for saving and loading sound files
- Added Oscillator, MicIn, Gain, ADSR, TimeStretch, PitchShift, and Analyser classes.


## [v0.1.32] - 2023-03-01

- Fix bugs


## [v0.1.31] - 2023-02-27

- Add ChangeLog.md file
- Add test.yml, tag.yaml, and release.yml
- Requires ruby 2.7.0 or later


## [v0.1.30] - 2023-02-09

- Fix conflicting beeps's Init_exception() and others Init_exception()
- Refactoring
