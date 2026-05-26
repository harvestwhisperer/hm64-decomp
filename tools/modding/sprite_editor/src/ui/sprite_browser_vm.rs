//! Discovers sprite assets under `<asset_root>/assets/sprites/**/` and builds
//! a two-level tree (category → optional group → sprite) for the collapsible

use std::collections::HashSet;
use std::path::{Path, PathBuf};

use crate::io::manifest::read_manifest;

#[derive(Debug, Clone)]
pub struct SpriteEntry {
    pub label: String,
    pub subdir: String,
    pub group: Option<String>,
    pub dir: PathBuf,
    pub texture_count: u32,
    pub palette_count: u32,
    pub animation_count: u32,
}

#[derive(Debug, Clone, Default)]
pub struct SpriteTree {
    pub sprites: Vec<SpriteEntry>,
    pub categories: Vec<CategoryNode>,
}

#[derive(Debug, Clone)]
pub struct CategoryNode {
    pub name: String,
    pub children: Vec<CategoryChild>,
}

#[derive(Debug, Clone)]
pub enum CategoryChild {
    Group { name: String, sprite_indices: Vec<usize> },
    Sprite(usize),
}

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum NodeKey {
    Category(String),
    Group { category: String, name: String },
}

impl NodeKey {
    pub fn encode(&self) -> String {
        match self {
            NodeKey::Category(c) => format!("cat:{c}"),
            NodeKey::Group { category, name } => format!("grp:{category}/{name}"),
        }
    }

    pub fn decode(s: &str) -> Option<Self> {
        if let Some(rest) = s.strip_prefix("cat:") {
            return Some(NodeKey::Category(rest.to_string()));
        }
        if let Some(rest) = s.strip_prefix("grp:") {
            let (cat, name) = rest.split_once('/')?;
            return Some(NodeKey::Group { category: cat.to_string(), name: name.to_string() });
        }
        None
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum TreeRowKind {
    Category = 0,
    Group = 1,
    Sprite = 2,
}

#[derive(Debug, Clone)]
pub struct TreeRow {
    pub depth: u8,
    pub kind: TreeRowKind,
    pub label: String,
    pub stats: String,
    pub expanded: bool,
    pub sprite_index: i32,
    pub category: String,
    pub group: Option<String>,
}

pub fn discover(asset_root: &Path) -> SpriteTree {
    let sprites_root = asset_root.join("assets/sprites");
    let mut sprites: Vec<SpriteEntry> = Vec::new();
    let mut categories: Vec<CategoryNode> = Vec::new();

    let Ok(cats) = std::fs::read_dir(&sprites_root) else {
        return SpriteTree { sprites, categories };
    };
    let mut cat_entries: Vec<_> = cats.flatten().filter(|e| e.path().is_dir()).collect();
    cat_entries.sort_by_key(|e| e.file_name());

    for cat_entry in cat_entries {
        let cat_path = cat_entry.path();
        let cat_name = cat_entry.file_name().to_string_lossy().into_owned();
        let mut children_named: Vec<(String, CategoryChild)> = Vec::new();

        let Ok(items) = std::fs::read_dir(&cat_path) else { continue };
        let mut item_paths: Vec<PathBuf> =
            items.flatten().map(|e| e.path()).filter(|p| p.is_dir()).collect();
        item_paths.sort();

        for path in item_paths {
            let Some(name_os) = path.file_name() else { continue };
            let name = name_os.to_string_lossy().into_owned();
            let manifest_path = path.join("manifest.json");
            if manifest_path.is_file() {
                if let Ok(m) = read_manifest(&manifest_path) {
                    let idx = sprites.len();
                    let display = m.label.clone();
                    sprites.push(SpriteEntry {
                        label: m.label,
                        subdir: m.subdir,
                        group: None,
                        dir: path.clone(),
                        texture_count: m.sprite_count,
                        palette_count: m.palette_count,
                        animation_count: m.animation_count,
                    });
                    children_named.push((display, CategoryChild::Sprite(idx)));
                }
            } else {
                // Treat as group: keep it in the tree as long as the dir
                // exists, even if it has no sprite children right now. That
                // way a user who dragged the last sprite out of a group still
                // sees the group as a drop target so they can put it back.
                let mut indices: Vec<usize> = Vec::new();
                let Ok(grp_items) = std::fs::read_dir(&path) else { continue };
                let mut grp_paths: Vec<PathBuf> =
                    grp_items.flatten().map(|e| e.path()).filter(|p| p.is_dir()).collect();
                grp_paths.sort();
                for gp in grp_paths {
                    let gmanifest = gp.join("manifest.json");
                    if !gmanifest.is_file() {
                        continue;
                    }
                    if let Ok(m) = read_manifest(&gmanifest) {
                        let idx = sprites.len();
                        sprites.push(SpriteEntry {
                            label: m.label,
                            subdir: m.subdir,
                            group: Some(name.clone()),
                            dir: gp.clone(),
                            texture_count: m.sprite_count,
                            palette_count: m.palette_count,
                            animation_count: m.animation_count,
                        });
                        indices.push(idx);
                    }
                }
                indices.sort_by(|&a, &b| sprites[a].label.cmp(&sprites[b].label));
                children_named.push((
                    name.clone(),
                    CategoryChild::Group { name: name.clone(), sprite_indices: indices },
                ));
            }
        }

        // Subfolders (groups) sort before sprite leaves so the tree reads
        // "folders, then sprites" within each category instead of mixing them
        // alphabetically.
        children_named.sort_by(|a, b| {
            let a_is_sprite = matches!(a.1, CategoryChild::Sprite(_));
            let b_is_sprite = matches!(b.1, CategoryChild::Sprite(_));
            a_is_sprite
                .cmp(&b_is_sprite)
                .then_with(|| a.0.to_ascii_lowercase().cmp(&b.0.to_ascii_lowercase()))
        });
        let children = children_named.into_iter().map(|(_, c)| c).collect();
        categories.push(CategoryNode { name: cat_name, children });
    }

    SpriteTree { sprites, categories }
}

pub fn default_expanded(tree: &SpriteTree) -> HashSet<NodeKey> {
    tree.categories.iter().map(|c| NodeKey::Category(c.name.clone())).collect()
}

pub fn flatten(
    tree: &SpriteTree,
    expanded: &HashSet<NodeKey>,
    filter: &str,
) -> Vec<TreeRow> {
    let filter_lc = filter.to_ascii_lowercase();
    let has_filter = !filter_lc.is_empty();
    let sprite_matches = |idx: usize| -> bool {
        if !has_filter {
            return true;
        }
        tree.sprites[idx].label.to_ascii_lowercase().contains(&filter_lc)
    };

    let mut out: Vec<TreeRow> = Vec::new();
    for cat in &tree.categories {
        let cat_key = NodeKey::Category(cat.name.clone());
        let cat_has_match = cat.children.iter().any(|child| match child {
            CategoryChild::Sprite(i) => sprite_matches(*i),
            CategoryChild::Group { sprite_indices, .. } => {
                sprite_indices.iter().any(|i| sprite_matches(*i))
            }
        });
        if has_filter && !cat_has_match {
            continue;
        }

        let cat_expanded = has_filter || expanded.contains(&cat_key);
        out.push(TreeRow {
            depth: 0,
            kind: TreeRowKind::Category,
            label: cat.name.clone(),
            stats: String::new(),
            expanded: cat_expanded,
            sprite_index: -1,
            category: cat.name.clone(),
            group: None,
        });
        if !cat_expanded {
            continue;
        }

        for child in &cat.children {
            match child {
                CategoryChild::Sprite(i) => {
                    if !sprite_matches(*i) {
                        continue;
                    }
                    let e = &tree.sprites[*i];
                    out.push(TreeRow {
                        depth: 1,
                        kind: TreeRowKind::Sprite,
                        label: e.label.clone(),
                        stats: format_stats(e),
                        expanded: false,
                        sprite_index: *i as i32,
                        category: cat.name.clone(),
                        group: None,
                    });
                }
                CategoryChild::Group { name, sprite_indices } => {
                    let grp_has_match =
                        sprite_indices.iter().any(|i| sprite_matches(*i));
                    if has_filter && !grp_has_match {
                        continue;
                    }
                    let gkey = NodeKey::Group {
                        category: cat.name.clone(),
                        name: name.clone(),
                    };
                    let grp_expanded = has_filter || expanded.contains(&gkey);
                    out.push(TreeRow {
                        depth: 1,
                        kind: TreeRowKind::Group,
                        label: name.clone(),
                        stats: String::new(),
                        expanded: grp_expanded,
                        sprite_index: -1,
                        category: cat.name.clone(),
                        group: Some(name.clone()),
                    });
                    if !grp_expanded {
                        continue;
                    }
                    for &i in sprite_indices {
                        if !sprite_matches(i) {
                            continue;
                        }
                        let e = &tree.sprites[i];
                        out.push(TreeRow {
                            depth: 2,
                            kind: TreeRowKind::Sprite,
                            label: e.label.clone(),
                            stats: format_stats(e),
                            expanded: false,
                            sprite_index: i as i32,
                            category: cat.name.clone(),
                            group: Some(name.clone()),
                        });
                    }
                }
            }
        }
    }
    out
}

fn format_stats(e: &SpriteEntry) -> String {
    format!(
        "{} · {} tex / {} pal / {} anim",
        e.subdir, e.texture_count, e.palette_count, e.animation_count
    )
}
