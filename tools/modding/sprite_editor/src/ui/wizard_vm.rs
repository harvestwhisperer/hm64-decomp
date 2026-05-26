//! New-Sprite wizard state held between opening the dialog and clicking
//! Create. No Slint types here; the view binds to plain strings/ints.

use std::path::PathBuf;

#[derive(Debug, Default)]
pub struct WizardState {
    pub category_index: i32,
    pub kind_index: i32,
    pub type_index: i32,
    pub name: String,
    pub seed_from_png: bool,
    pub picked_pngs: Vec<PathBuf>,
}

impl WizardState {
    pub fn category(&self) -> &'static str {
        match self.category_index {
            0 => "entitySprites",
            1 => "mapObjects",
            2 => "effects",
            3 => "overlayScreens",
            _ => "entitySprites",
        }
    }
    pub fn kind(&self) -> crate::model::SpriteKind {
        match self.kind_index {
            1 => crate::model::SpriteKind::Avatar,
            2 => crate::model::SpriteKind::Standalone,
            _ => crate::model::SpriteKind::Entity,
        }
    }
    pub fn sprite_type(&self) -> crate::model::SpriteType {
        match self.type_index {
            0 => crate::model::SpriteType::Type1,
            _ => crate::model::SpriteType::Type2,
        }
    }
    pub fn validate(&self, asset_root: &std::path::Path) -> Result<(), String> {
        if self.name.is_empty() {
            return Err("Name is required".into());
        }
        if self.name.contains('/') || self.name.contains('\\') || self.name.contains(' ') {
            return Err("Name must be a single identifier (no spaces or path separators)".into());
        }
        let dir = asset_root.join("assets/sprites").join(self.category()).join(&self.name);
        if dir.exists() {
            return Err(format!("{} already exists", dir.display()));
        }
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tempfile::tempdir;

    #[test]
    fn validate_rejects_empty_name() {
        let dir = tempdir().unwrap();
        let state = WizardState::default();
        let err = state.validate(dir.path()).unwrap_err();
        assert!(err.contains("required"), "expected 'required' in error, got: {err}");
    }

    #[test]
    fn validate_rejects_name_with_space() {
        let dir = tempdir().unwrap();
        let state = WizardState { name: "my sprite".into(), ..Default::default() };
        let err = state.validate(dir.path()).unwrap_err();
        assert!(
            err.contains("single identifier"),
            "expected 'single identifier' in error, got: {err}"
        );
    }

    #[test]
    fn validate_rejects_name_with_slash() {
        let dir = tempdir().unwrap();
        let state = WizardState { name: "foo/bar".into(), ..Default::default() };
        let err = state.validate(dir.path()).unwrap_err();
        assert!(
            err.contains("single identifier"),
            "expected 'single identifier' in error, got: {err}"
        );
    }

    #[test]
    fn validate_rejects_name_with_backslash() {
        let dir = tempdir().unwrap();
        let state = WizardState { name: "foo\\bar".into(), ..Default::default() };
        let err = state.validate(dir.path()).unwrap_err();
        assert!(
            err.contains("single identifier"),
            "expected 'single identifier' in error, got: {err}"
        );
    }

    #[test]
    fn validate_ok_with_nonexistent_path() {
        // `tempdir` creates a real dir but the sprite subpath doesn't exist yet.
        let dir = tempdir().unwrap();
        let state = WizardState { name: "brandnew".into(), ..Default::default() };
        // Default category_index = 0 -> entitySprites.
        state.validate(dir.path()).expect("validate should succeed for fresh name + path");
    }

    #[test]
    fn category_mapping() {
        let mut s = WizardState { category_index: 0, ..Default::default() };
        assert_eq!(s.category(), "entitySprites");
        s.category_index = 1;
        assert_eq!(s.category(), "mapObjects");
        s.category_index = 2;
        assert_eq!(s.category(), "effects");
        s.category_index = 3;
        assert_eq!(s.category(), "overlayScreens");
        s.category_index = 99;
        assert_eq!(s.category(), "entitySprites");
    }

    #[test]
    fn kind_mapping() {
        let mut s = WizardState { kind_index: 0, ..Default::default() };
        assert_eq!(s.kind(), crate::model::SpriteKind::Entity);
        s.kind_index = 1;
        assert_eq!(s.kind(), crate::model::SpriteKind::Avatar);
        s.kind_index = 2;
        assert_eq!(s.kind(), crate::model::SpriteKind::Standalone);
        s.kind_index = 99;
        assert_eq!(s.kind(), crate::model::SpriteKind::Entity);
    }
}
