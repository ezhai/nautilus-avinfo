import { defineConfig } from "vitepress";

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "Nautilus AVInfo",
  description: "Documentation for the Nautilus AVInfo extension.",
  base: "/nautilus-avinfo/",
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: "Home", link: "/" },
      { text: "Guide", link: "/guide/installation" },
      { text: "Reference", link: "/developer/getting-started" },
    ],
    sidebar: [
      {
        text: "User Guide",
        items: [
          { text: "Installation", link: "/guide/installation" },
          { text: "Usage", link: "/guide/usage" },
        ],
      },
      {
        text: "Developer Guide",
        items: [
          { text: "Getting Started", link: "/reference/getting-started" },
          { text: "Release Info", link: "/reference/release" },
          {
            text: "RPM Packaging Notes",
            link: "/reference/rpm-packaging-notes",
          },
        ],
      },
    ],
    socialLinks: [
      { icon: "github", link: "https://github.com/ezhai/nautilus-avinfo" },
    ],
  },
});
