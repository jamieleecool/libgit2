#include "clar_libgit2.h"

#include "git2/clone.h"
#include "repository.h"

static git_clone_options g_options;
static git_repository *g_repo;
static git_repository *g_repo_cloned;

void test_clone_empty__initialize(void)
{
	git_repository *sandbox = cl_git_sandbox_init("empty_bare.git");
	cl_git_remove_placeholders(git_repository_path(sandbox), "dummy-marker.txt");

	g_repo = NULL;

	memset(&g_options, 0, sizeof(git_clone_options));
	g_options.version = GIT_CLONE_OPTIONS_VERSION;
}

void test_clone_empty__cleanup(void)
{
	cl_git_sandbox_cleanup();
}

static void cleanup_repository(void *path)
{
	cl_fixture_cleanup((const char *)path);

	git_repository_free(g_repo_cloned);
	g_repo_cloned = NULL;
}

void test_clone_empty__can_clone_an_empty_local_repo_barely(void)
{
	char *local_name = "refs/heads/master";
	char tracking_name[1024];
	git_reference *ref;

	cl_set_cleanup(&cleanup_repository, "./empty");

	g_options.bare = true;
	cl_git_pass(git_clone(&g_repo_cloned, "./empty_bare.git", "./empty", &g_options));

	/* Although the HEAD is orphaned... */
	cl_assert_equal_i(GIT_ENOTFOUND, git_reference_lookup(&ref, g_repo_cloned, local_name));

	/* ...one can still retrieve the name of the remote tracking reference */
	cl_assert_equal_i((int)strlen("refs/remotes/origin/master") + 1U, 
		git_branch_tracking_name(tracking_name, 1024, g_repo_cloned, local_name));
}

void test_clone_empty__can_clone_an_empty_local_repo(void)
{
	cl_set_cleanup(&cleanup_repository, "./empty");

	cl_git_pass(git_clone(&g_repo_cloned, "./empty_bare.git", "./empty", &g_options));
}

void test_clone_empty__can_clone_an_empty_standard_repo(void)
{
	cl_git_sandbox_cleanup();
	g_repo = cl_git_sandbox_init("empty_standard_repo");
	cl_git_remove_placeholders(git_repository_path(g_repo), "dummy-marker.txt");

	cl_set_cleanup(&cleanup_repository, "./empty");

	cl_git_pass(git_clone(&g_repo_cloned, "./empty_standard_repo", "./empty", &g_options));
}
